/**
 * @file    stacktrace.c
 * @author  Théo Bessel
 * @brief   Interface for stack trace handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "fdir/fdir.h"
#include "core/tasks.h"
#include "system/console.h"
#include "system/sysleds.h"
#include "utils/log.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

// CantUnwind symbol
#define EXIDX_CANTUNWIND 0x1

// Personality routine indexes
#define SU16 0x0
#define LU16 0x1
#define LU32 0x2

// debugInfo_t manipulation
#define LAST_CALL(call_stack) call_stack->calls[call_stack->size]

// Masks
#define SIX_RIGHT_MASK(instruction) ((instruction & 0x3f) << 2)

/*************************** Functions Declarations **************************/

void UnwindStack(callStack_t* call_stack, call_t last_call);
void UnwindNextFrame(callStack_t* call_stack);

uint32_t __attribute__((pure)) DecodeFrame(uint32_t entry, uint32_t decoded_entry, uint32_t fp);
uint32_t __attribute__((pure)) DecodeCompactModelEntry(const uint32_t entry, const uint32_t word, const uint32_t fp, const uint8_t instr_count, const uint8_t offset);
uint32_t __attribute__((pure)) GetInstruction(const uint32_t entry, const uint32_t word, const uint8_t offset, const uint8_t offset2);
exidxEntry_t __attribute__((pure)) GetExidxEntry(const uint8_t* const section, const uint32_t offset);
uint32_t __attribute__((pure)) DecodePrel31(const uint32_t word, const uint32_t where);
uint32_t __attribute__((pure)) GetWord(const uint8_t* const section, const uint32_t offset);

/*************************** Handlers Declarations ***************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @brief This function makes an unwind to compute the stacktrace from the program
 * counter variable.
 * @param[out] call_stack             The structure where to store the stracktrace
 * @param[in] last_call               The unwind context (lr + fp)
 * @return Nothing
 */
void UnwindStack(callStack_t* call_stack, call_t last_call)
{
    call_stack->size = 0;

    // Setup last call
    LAST_CALL(call_stack) = last_call;

    while (
        call_stack->size < CALL_STACK_MAX_SIZE
        && LAST_CALL(call_stack).lr != 0xffffffff
        && LAST_CALL(call_stack).fp != 0x07070707
    )
    {
        UnwindNextFrame(call_stack);
    }
}

/**
 * @brief This function unwind the frame following the last valid address stored
 * in call_stack
 * @param[out] call_stack     The structure where to store the frame computed lr
 * @return Nothing
 */
void UnwindNextFrame(callStack_t* call_stack)
{
    /**
     * @brief Total number of entries in the unwind table
     */
    uint8_t entries_count = (&__exidx_end - &__exidx_start) / 2;

    /**
     * @brief Unwind tables entries
     */
    uint32_t extab_entry = 0x0;
    exidxEntry_t entry;

    /**
     * @brief Frame pointer
     */
    uint32_t fp = LAST_CALL(call_stack).fp;

    // New frame pointer
    uint32_t new_fp = 0x0;

    /**
     * Iterate over all entries, get the function return address and find the entry
     * corresponding to the last return address unwound (ie. the address of the function
     * associated with the frame to unwind).
     *
     * TODO: Could be optimized with a dichotomic search because addresses are sorted in
     * unwind table. The complexity would then be O(log_2(N)) instead of O(N)
     */
    do {
        entries_count--;
        entry = GetExidxEntry((uint8_t *)&__exidx_start, 8 * entries_count);
    } while (
        entries_count > 0
        && entry.decoded_fn > LAST_CALL(call_stack).lr
    );

    // TODO : See if remove this is interesting to have the exact instruction of the error
    LAST_CALL(call_stack).lr = entry.decoded_fn;

    /**
     * Move to the next call array place.
     */
    call_stack->size += 1;

    /**
     * (Section 6)
     * The second word contains one of:
     *   - The prel31 offset of the start of the table entry for this function, with bit 31 clear.
     *   - The exception-handling table entry itself with bit 31 set, if it can be encoded in 31 bits
     *     (see The Arm-defined compact model).
     *   - The special bit pattern EXIDX_CANTUNWIND (0x1), indicating to run-time support code that associated
     *     frames cannot be unwound. On encountering this pattern the language-independent unwinding routines
     *     return a failure code to their caller, which should take an appropriate action such as calling
     *     terminate() or abort(). See Phase 1 unwinding and Phase 2 unwinding.
     */
    if (entry.exidx_entry == EXIDX_CANTUNWIND)      // Special pattern 0x1 EXIDX_CANTUNWIND
    {
        LAST_CALL(call_stack).lr = 0xffffffff;
        LAST_CALL(call_stack).fp = 0xffffffff;
    }
    else if (entry.exidx_entry & 0x80000000)        // Bit 31 set --> compact model
    {
        new_fp = DecodeFrame(entry.exidx_entry, entry.decoded_entry, fp);

        /**
         * The `lr` register is pushed just before the `fp` register, then we can get it by accessing `fp + 4`
         */
        LAST_CALL(call_stack).lr = *((uint32_t *) (new_fp + 4)) - 1;
        LAST_CALL(call_stack).fp = *((uint32_t *) new_fp);
    }
    else                                            // Bit 31 is clear
    {
        extab_entry = GetWord((uint8_t *) entry.decoded_entry, 0);

        if (extab_entry & 0x80000000)
        {
            new_fp = DecodeFrame(extab_entry, entry.decoded_entry, fp);

            /**
             * The `lr` register is pushed just before the `fp` register, then we can get it by accessing `fp + 4`
             */
            LAST_CALL(call_stack).lr = *((uint32_t *) (new_fp + 4)) - 1;
            LAST_CALL(call_stack).fp = *((uint32_t *) new_fp);
        }
    }
}

/**
 * @brief This function execute the personnality routine for a given entry and
 * return the computed frame pointer
 * @warning This function is annotated with the `pure` attribute for
 * performance purpose, it must remain pure if it's changed
 * @param[in] entry the exidx/extab entryn(depending on its format) of the frame to decode
 * @param[in] decoded_entry the decoded exidx entry of the frame to decode
 * @param[in] fp the old frame pointer (used to unwind the next step)
 * @return The new frame pointer
 */
uint32_t __attribute__((pure)) DecodeFrame(const uint32_t entry, const uint32_t decoded_entry, const uint32_t fp) {
    /**
     * (Section 10.2)
     * The first word is as described in The Arm-defined compact model.
     * The most significant byte encodes the index of the personality routine (PR) used
     * to interpret what follows :
     *   0 - Su16 : Short frame unwinding description followed by descriptors with 16-bit scope.
     *   1 - Lu16 : Long frame unwinding description followed by descriptors with 16-bit scope.
     *   2 - Lu32 : Long frame unwinding description followed by descriptors with 32-bit scope.
     */
    uint32_t word = (entry & 0xffffff);

    /**
     * @brief Number of instructions to decode the frame
     */
    uint32_t instr_count = (word >> 16) & 0xff;

    // Initialize the new frame pointer
    uint32_t new_fp = fp;

    /**
     * (Section 7.3)
     * An exception-handling table entry for the compact model looks like:
     * 31 | 30 - 28 | 27 - 24 | 23 ------------------------------- 0 |
     *  1 |       0 |   index |  Data for personalityRoutine[index]. |
     *
     * The personality routine is the set of instructions required to unwind the stack.
     *
     * (Section 7.3)
     * Arm has allocated index numbers 0, 1 and 2 for use by C and C++.
     * Arm-defined personality routines and table formats for C and C++ details the mapping
     * from index numbers to personality routines and explains how to use them.
     * Index numbers 3-15 are reserved for future use.
     *
     *
     * Thus, personality index for `entry` is `(uint8_t) ((entry >> 24) & 0xf)`
     */
    switch ((uint8_t) ((entry >> 24) & 0xf))
    {
        /**
         * (Section 10.2)
         * Short 3 unwinding instructions in bits 16-23, 8-15, and 0-7 of the first word. Any of the instructions can be Finish.
         */
        case SU16:
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 3, 1);
            break;
        /**
         * (Section 10.2)
         * Long Bits 16-23 contain a count N of the number of additional 4-byte words that contain unwinding instructions.
         * The sequence of unwinding instructions is packed into bits 8-15, 0-7, and the following N words.
         * Spare trailing bytes in the last word should be filled with Finish instructions.
         */
        case LU16:
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 2 + 4 * instr_count, 2);
            break;
        case LU32:
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 2 + 4 * instr_count, 2);
            break;
        default:
            // error
            break;
    }

    return new_fp;
}

/**
 * @brief This function decodes unwind instructions based on ARM EHABI standard.
 * @warning This function is annotated with the `pure` attribute for
 * performance purpose, it must remain pure if it's changed
 * @param[in] entry_ptr the address of the words to decode
 * @param[in] word the original word decoded
 * @param[in] fp the old frame pointer
 * @param[in] instr_count the number of instructions
 * @param[in] offset a specific offset within the word (= 1 or 2 depending of the compact model index)
 * @return The new frame pointer
 */
uint32_t __attribute__((pure)) DecodeCompactModelEntry(const uint32_t entry_ptr, const uint32_t word, const uint32_t fp, const uint8_t instr_count, const uint8_t offset)
{
    // Instructions to fetch
    uint32_t instr1 = 0x0;
    uint32_t instr2 = 0x0;

    // Instruction counter
    uint8_t instr_index = 0x0;

    // New frame pointer
    uint32_t new_fp = fp;

    // Condition representing whether there is a second instruction to fetch (because of the posibility to fetch two instructions in one)
    uint8_t double_instr = 0x0;

    // Loop while there are instructions to fetch
    while (instr_index < instr_count)
    {
        double_instr = instr_index < instr_count - 1;

        // Fetch the two first instructions
        instr1 = GetInstruction(entry_ptr, word, instr_index, offset);
        instr2 = double_instr ? GetInstruction(entry_ptr, word, instr_index + 1, offset) : 0;

        // Decode these instructions
        if      ((instr1 & 0xc0) == 0x00)
        {
            /**
             * @brief 00xxxxxx
             * vsp = vsp + (xxxxxx << 2) + 4. Covers range 0x04-0x100 inclusive
             */
            new_fp += SIX_RIGHT_MASK(instr1) + 4;
        }
        else if ((instr1 & 0xc0) == 0x40)
        {
            /**
             * @brief 01xxxxxx
             * vsp = vsp – (xxxxxx << 2) - 4. Covers range 0x04-0x100 inclusive
             */
            new_fp -= SIX_RIGHT_MASK(instr1) - 4;
        }
        else if (double_instr && (instr1 == 0x80) && (instr2 == 0x00))             { instr_index++; }
        else if (double_instr && (instr1 & 0xf0) == 0x80)                          { instr_index++; }
        else if ((instr1 == 0x9d))                                                 { }
        else if ((instr1 == 0x9f))                                                 { }
        else if ((instr1 & 0xf0) == 0x90)                                          { }
        else if ((instr1 & 0xf8) == 0xa0)                                          { }
        else if ((instr1 & 0xf8) == 0xa8)                                          { }
        else if ((instr1 == 0xb0))                                                 { }
        else if (double_instr && (instr1 == 0xb1) && (instr2 == 0x00))             { instr_index++; }
        else if (double_instr && (instr1 == 0xb1) && ((instr2 & 0xf0) == 0x00))    { instr_index++; }
        else if (double_instr && (instr1 == 0xb1))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xb2))
        {
            /**
             * @brief 10110010 uleb128
             * vsp = vsp + 0x204+ (uleb128 << 2) (for vsp increments of 0x104-0x200, use 00xxxxxx twice)
             */
            new_fp += 0x204 + (instr2 << 2);
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xb3))                                 { instr_index++; }
        else if ((instr1 == 0xb4))                                                 { }
        else if ((instr1 & 0xf8) == 0xb8)                                          { }
        else if ((instr1 & 0xf8) == 0xc0)                                          { }
        else if (double_instr && (instr1 == 0xc6))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xc7) && (instr2 == 0x00))             { instr_index++; }
        else if (double_instr && (instr1 == 0xc7) && ((instr2 & 0xf0) == 0x00))    { instr_index++; }
        else if (double_instr && (instr1 == 0xc7))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xc8))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xc9))                                 { instr_index++; }
        else if ((instr1 & 0xf8) == 0xc8)                                          { }
        else if ((instr1 & 0xf8) == 0xd0)                                          { }
        else if ((instr1 & 0xc0) == 0xc0)                                          { }
        else { }

        instr_index++;
    }

    return new_fp;
}

/**
 * @brief This function fetches an unwind instruction given in parameter.
 * @warning This function is annotated with the `pure` attribute for
 * performance purpose, it must remain pure if it's changed
 * @param[in] entry_ptr the address of the words to decode
 * @param[in] word the original word decoded
 * @param[in] offset the offset in the section
 * @param[in] offset2 the offset in the word
 * @return The instruction contained at address of entry_ptr with given offsets
 */
uint32_t __attribute__((pure)) GetInstruction(const uint32_t entry_ptr, const uint32_t word, const uint8_t offset, const uint8_t offset2)
{
    uint32_t instr = 0x0;
    uint32_t new_word = word;

    // Calculate which word we need to access based on the offset
    if (offset >= 4 - offset2)
    {
        // Fetch a new word from memory using GetWord when offset crosses word boundaries
        new_word = GetWord((uint8_t *) entry_ptr, 4 * ((offset - offset2) / 4 + 1));

        // A bit of magic calculations
        instr = (new_word >> (24 - ((offset - offset2) % 4) * 8)) & 0xff;
    } else {
        // A bit of magic calculations
        instr = (new_word >> (24 - ((offset + offset2) % 4) * 8)) & 0xff;
    }

    return instr;
}

/**
 * @brief This function decodes an entry in the Exidx (Exception Index) Table.
 * @warning This function is annotated with the `pure` attribute for
 * performance purpose, it must remain pure if it's changed
 * @param[in] section
 * @param[in] offset
 * @return The exidx entry in both raw and decoded forms (exidxEntry_t)
 */
exidxEntry_t __attribute__((pure)) GetExidxEntry(const uint8_t* const section, const uint32_t offset)
{
    exidxEntry_t entry;
    entry.exidx_fn = GetWord(section, offset);
    entry.exidx_entry = GetWord(section, offset+4);

    /**
     * (Section 6)
     * The first word contains a prel31 offset (see Relocations) to the start of a function, with bit 31 clear.
    */
    entry.decoded_fn = entry.exidx_fn & 0x80000000
        ? 0
        : DecodePrel31(entry.exidx_fn, (uint32_t) section + offset);

    entry.decoded_entry = entry.exidx_entry & 0x80000000
        ? entry.exidx_entry
        : DecodePrel31(entry.exidx_entry, (uint32_t) section + offset + 4);

    return entry;
}

/**
 * @brief This decodes an offset with prel31 encoding.
 * @warning This function is annotated with the `pure` attribute for
 * performance purpose, it must remain pure if it's changed
 * @param[in] word
 * @param[in] where
 * @return The prel31 offset of the word given in parameter
 */
uint32_t __attribute__((pure)) DecodePrel31(const uint32_t word, const uint32_t where)
{
    // Get the 31 weak bits of the word
    uint32_t offset = word & 0x7fffffff;

    // Use the 31st bit as sign bit
    if (offset & 0x40000000)
    {
        offset |= ~ (uint64_t) 0x7fffffff;
    }

    // Add the relocation
    offset += where;

    return offset;
}

/**
 * @brief This gets a word in a given offset of the section in parameter.
 * @warning This function is annotated with the `pure` attribute for
 * performance purpose, it must remain pure if it's changed
 * @param[in] section
 * @param[in] offset
 * @return The 32-bit word at the specified offset.
 */
uint32_t __attribute__((pure)) GetWord(const uint8_t* const section, const uint32_t offset)
{
    uint8_t const* const field = section + offset;

    return (
        field[0]
        | (field[1] << 8)
        | (field[2] << 16)
        | (field[3] << 24)
    );
}
