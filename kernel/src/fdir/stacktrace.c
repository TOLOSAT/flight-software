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

// Unwind Limits
#define LR_STOP_UNWIND  0xffffffffu /**< Last LR to which it is possible to unwind knowing that the signature of an exception return  */
#define FP_STOP_UNWIND  0x07070707u /**< Last FP to which it is possible to unwind knowing that the stack of a task is initialised with r7 = 0x07070707u */

// ARM unwind specific constant
#define COMPACT_MODEL_BITMASK   0x80000000u /**< Exidx entry bitmask that indicates if using compact model or not */
#define EXIDX_CANTUNWIND        0x1u        /**< Can't Unwind Symbol */
#define SU16                    0x0u        /**< SU16 personality routine index */
#define LU16                    0x1u        /**< LU16 personality routine index */
#define LU32                    0x2u        /**< LU32 personality routine index */

/**
 * @def     GET_INSTR_6LSB(instruction)
 * @brief   Preprocessor function that gets the 6 LSBs of an instruction
 */
#define GET_INSTR_6LSB(instruction) (((instruction) & 0x3fu) << 2) /**< Mask for getting the 6 LSB */

/**
 * @def     LAST_CALL(call_stack)
 * @brief   Preprocessor function that gets the last call in the stack trace
 */
#define LAST_CALL(call_stack) ((call_stack)->calls[(call_stack)->last_idx])

/*************************** Functions Declarations **************************/

static void UnwindNextFrame(callStack_t* call_stack);

static uint32_t DecodeFrame(uint32_t entry, uint32_t decoded_entry, uint32_t fp);
static uint32_t DecodeCompactModelEntry(const uint32_t entry, const uint32_t word, const uint32_t fp, const uint32_t instr_count, const uint32_t offset);
static uint32_t GetInstruction(const uint32_t entry, const uint32_t word, const uint32_t offset, const uint32_t offset2);
static exidxEntry_t GetExidxEntry(const uint8_t* const section, const uint32_t offset);
static uint32_t DecodePrel31(const uint32_t word, const uint32_t where);
static uint32_t GetWord(const uint8_t* const section, const uint32_t offset);

/*************************** Variables Definitions ***************************/

extern uint32_t __exidx_start;
extern uint32_t __exidx_end;

/*************************** Functions Definitions ***************************/

/**
 * @fn          UnwindStackFromContext(callStack_t* call_stack, call_t last_call)
 * @brief       This function makes an unwind to compute the stacktrace from the program counter variable.
 * @param[out]  call_stack  The structure where to store the stracktrace
 * @param[in]   last_call   The unwind context (lr + fp)
 * @return      Nothing
 */
void UnwindStackFromContext(callStack_t* call_stack, call_t last_call)
{
    call_stack->last_idx = 0u;

    // Setup last call
    LAST_CALL(call_stack) = last_call;

    while (
        (call_stack->last_idx < CALL_STACK_MAX_SIZE)
        && (LAST_CALL(call_stack).lr != LR_STOP_UNWIND)
        && (LAST_CALL(call_stack).fp != FP_STOP_UNWIND)
    )
    {
        UnwindNextFrame(call_stack);
    }
}

/**
 * @fn          UnwindNextFrame(callStack_t* call_stack)
 * @brief       This function unwind the frame following the last valid address stored in call_stack
 * @param[out]  call_stack  The structure where to store the frame computed lr
 * @return      Nothing
 */
static void UnwindNextFrame(callStack_t* call_stack)
{

    // Total number of entries in the unwind table
    uint32_t entries_count = ((uint32_t)&__exidx_end - (uint32_t)&__exidx_start) / (2u * sizeof(uint32_t)); // cppcheck-suppress misra-c2012-11.4; Not ideal but the only way to know the section size and thus number of entries
    // Unwind tables entries
    exidxEntry_t entry = {0};

    // Frame pointer
    uint32_t fp = LAST_CALL(call_stack).fp;

    // New frame pointer
    uint32_t new_fp;

    // Iterate over all entries, get the function return address and find the entry
    // corresponding to the last return address unwound (ie. the address of the function
    // associated with the frame to unwind).
    //
    // TO DO : Could be optimized with a dichotomic search because addresses are sorted in
    // unwind table. The complexity would then be O(log_2(N)) instead of O(N)
    do {
        entries_count--;
        entry = GetExidxEntry((uint8_t *)&__exidx_start, 8u * entries_count);
    } while (
        (entries_count > 0u)
        && (entry.decoded_fn > LAST_CALL(call_stack).lr)
    );

    // TO DO : See if remove this is interesting to have the exact instruction of the error
    LAST_CALL(call_stack).lr = entry.decoded_fn;

    // Move to the next call array place.    
    call_stack->last_idx++;

    // (Section 6)
    // The second word contains one of:
    //   - The prel31 offset of the start of the table entry for this function, with bit 31 clear.
    //   - The exception-handling table entry itself with bit 31 set, if it can be encoded in 31 bits
    //     (see The Arm-defined compact model).
    //   - The special bit pattern EXIDX_CANTUNWIND (0x1), indicating to run-time support code that associated
    //     frames cannot be unwound. On encountering this pattern the language-independent unwinding routines
    //     return a failure code to their caller, which should take an appropriate action such as calling
    //     terminate() or abort(). See Phase 1 unwinding and Phase 2 unwinding.
    if (entry.exidx_entry == EXIDX_CANTUNWIND)      // Special pattern 0x1 EXIDX_CANTUNWIND
    {
        LAST_CALL(call_stack).lr = LR_STOP_UNWIND;
        LAST_CALL(call_stack).fp = LR_STOP_UNWIND;
    }
    else if ((entry.exidx_entry & COMPACT_MODEL_BITMASK) != 0u)        // Bit 31 set --> compact model
    {
        new_fp = DecodeFrame(entry.exidx_entry, entry.decoded_entry, fp);

        /**
         * The `lr` register is pushed just before the `fp` register, then we can get it by accessing `fp + 4`
         */
        LAST_CALL(call_stack).lr = *((uint32_t *) (new_fp + 4u)) - 1u;
        LAST_CALL(call_stack).fp = *((uint32_t *) new_fp);
    }
    else                                            // Bit 31 is clear
    {
        uint32_t extab_entry = GetWord((uint8_t *) entry.decoded_entry, 0u);

        if ((extab_entry & COMPACT_MODEL_BITMASK) != 0u)
        {
            new_fp = DecodeFrame(extab_entry, entry.decoded_entry, fp);

            /**
             * The `lr` register is pushed just before the `fp` register, then we can get it by accessing `fp + 4`
             */
            LAST_CALL(call_stack).lr = *((uint32_t *) (new_fp + 4u)) - 1u;
            LAST_CALL(call_stack).fp = *((uint32_t *) new_fp);
        }
    }
}

/**
 * @fn          DecodeFrame(const uint32_t entry, const uint32_t decoded_entry, const uint32_t fp)
 * @brief       This function execute the personnality routine for a given entry and return the computed frame pointer
 * @param[in]   entry           The exidx/extab entryn(depending on its format) of the frame to decode
 * @param[in]   decoded_entry   The decoded exidx entry of the frame to decode
 * @param[in]   fp              The old frame pointer (used to unwind the next step)
 * @return      The new frame pointer
 * 
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE DecodeFrame(const uint32_t entry, const uint32_t decoded_entry, const uint32_t fp)
{
    // (Section 10.2)
    // The first word is as described in The Arm-defined compact model.
    // The most significant byte encodes the index of the personality routine (PR) used
    // to interpret what follows :
    //   0 - Su16 : Short frame unwinding description followed by descriptors with 16-bit scope.
    //   1 - Lu16 : Long frame unwinding description followed by descriptors with 16-bit scope.
    //   2 - Lu32 : Long frame unwinding description followed by descriptors with 32-bit scope.
    uint32_t word = (entry & 0x00ffffffu);

    // Number of instructions to decode the frame
    uint32_t instr_count = (word >> 16) & 0x000000ffu;

    // Initialize the new frame pointer
    uint32_t new_fp = fp;

    // (Section 7.3)
    // An exception-handling table entry for the compact model looks like:
    // 31 | 30 - 28 | 27 - 24 | 23 ------------------------------- 0 |
    //  1 |       0 |   index |  Data for personalityRoutine[index]. |
    //
    // The personality routine is the set of instructions required to unwind the stack.
    //
    // (Section 7.3)
    // Arm has allocated index numbers 0, 1 and 2 for use by C and C++.
    // Arm-defined personality routines and table formats for C and C++ details the mapping
    // from index numbers to personality routines and explains how to use them.
    // Index numbers 3-15 are reserved for future use.
    //
    //
    // Thus, personality index for `entry` is `(uint8_t) ((entry >> 24) & 0xf)`
    switch ((uint8_t) ((entry >> 24) & 0xfu))
    {
        // (Section 10.2)
        // Short 3 unwinding instructions in bits 16-23, 8-15, and 0-7 of the first word. Any of the instructions can be Finish.
        case SU16:
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 3u, 1u);
            break;

        // (Section 10.2)
        // Long Bits 16-23 contain a count N of the number of additional 4-byte words that contain unwinding instructions.
        // The sequence of unwinding instructions is packed into bits 8-15, 0-7, and the following N words.
        // Spare trailing bytes in the last word should be filled with Finish instructions.
        case LU16:
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 2u + (4u * instr_count), 2u);
            break;
        case LU32:
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 2u + (4u * instr_count), 2u);
            break;
        default:
            // error
            break;
    }

    return new_fp;
}

/**
 * @fn          DecodeCompactModelEntry(const uint32_t entry_ptr, const uint32_t word, const uint32_t fp, const uint32_t instr_count, const uint32_t offset)
 * @brief       This function decodes unwind instructions based on ARM EHABI standard.
 * @param[in]   entry_ptr   The address of the words to decode
 * @param[in]   word        The original word decoded
 * @param[in]   fp          The old frame pointer
 * @param[in]   instr_count The number of instructions
 * @param[in]   offset      A specific offset within the word (= 1 or 2 depending of the compact model index)
 * @return      The new frame pointer
 * 
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE DecodeCompactModelEntry(const uint32_t entry_ptr, const uint32_t word, const uint32_t fp, const uint32_t instr_count, const uint32_t offset)
{
    // Instruction counter
    uint32_t instr_index = 0x0u;

    // New frame pointer
    uint32_t new_fp = fp;

    // Loop while there are instructions to fetch
    while (instr_index < instr_count)
    {
        // Condition representing whether there is a second instruction to fetch (because of the posibility to fetch two instructions in one)
        bool double_instr = instr_index < (instr_count - 1u);

        // Fetch the two first instructions
        uint32_t instr1 = GetInstruction(entry_ptr, word, instr_index, offset);
        uint32_t instr2 = 0u;
        if (double_instr) 
        {
            instr2 = GetInstruction(entry_ptr, word, instr_index + 1u, offset);
        }

        // Decode these instructions
        if ((instr1 & 0xc0u) == 0x00u)
        {
            // 00xxxxxx
            // vsp = vsp + (xxxxxx << 2) + 4. Covers range 0x04-0x100 inclusive
            new_fp += GET_INSTR_6LSB(instr1) + 4u;
        }
        else if ((instr1 & 0xc0u) == 0x40u)
        {
            // 01xxxxxx
            // vsp = vsp – (xxxxxx << 2) - 4. Covers range 0x04-0x100 inclusive
            new_fp -= GET_INSTR_6LSB(instr1) - 4u;
        }
        else if (double_instr && (instr1 == 0x80u) && (instr2 == 0x00u))            { instr_index++; }
        else if (double_instr && (instr1 & 0xf0u) == 0x80u)                         { instr_index++; }
        else if ((instr1 == 0x9du))                                                 { }
        else if ((instr1 == 0x9fu))                                                 { }
        else if ((instr1 & 0xf0u) == 0x90u)                                         { }
        else if ((instr1 & 0xf8u) == 0xa0u)                                         { }
        else if ((instr1 & 0xf8u) == 0xa8u)                                         { }
        else if ((instr1 == 0xb0u))                                                 { }
        else if (double_instr && (instr1 == 0xb1u) && (instr2 == 0x00u))            { instr_index++; }
        else if (double_instr && (instr1 == 0xb1u) && ((instr2 & 0xf0u) == 0x00u))  { instr_index++; }
        else if (double_instr && (instr1 == 0xb1u))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xb2u))
        {
            // 10110010 uleb128
            // vsp = vsp + 0x204+ (uleb128 << 2) (for vsp increments of 0x104-0x200, use 00xxxxxx twice)
            new_fp += 0x204u + (instr2 << 2);
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xb3u))                                 { instr_index++; }
        else if ((instr1 == 0xb4u))                                                 { }
        else if ((instr1 & 0xf8u) == 0xb8u)                                         { }
        else if ((instr1 & 0xf8u) == 0xc0u)                                         { }
        else if (double_instr && (instr1 == 0xc6u))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xc7u) && (instr2 == 0x00u))            { instr_index++; }
        else if (double_instr && (instr1 == 0xc7u) && ((instr2 & 0xf0u) == 0x00u))  { instr_index++; }
        else if (double_instr && (instr1 == 0xc7u))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xc8u))                                 { instr_index++; }
        else if (double_instr && (instr1 == 0xc9u))                                 { instr_index++; }
        else if ((instr1 & 0xf8u) == 0xc8u)                                         { }
        else if ((instr1 & 0xf8u) == 0xd0u)                                         { }
        else if ((instr1 & 0xc0u) == 0xc0u)                                         { }
        else { }

        instr_index++;
    }

    return new_fp;
}

/**
 * @fn          GetInstruction(const uint32_t entry_ptr, const uint32_t word, const uint32_t offset, const uint32_t offset2)
 * @brief       This function fetches an unwind instruction given in parameter.
 * @param[in]   entry_ptr   The address of the words to decode
 * @param[in]   word        The original word decoded
 * @param[in]   offset      The offset in the section
 * @param[in]   offset2     The offset in the word
 * @return      The instruction contained at address of entry_ptr with given offsets
 * 
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE GetInstruction(const uint32_t entry_ptr, const uint32_t word, const uint32_t offset, const uint32_t offset2)
{
    uint32_t instr = 0x0u;
    uint32_t new_word = word;

    // Calculate which word we need to access based on the offset
    if (offset >= (4u - offset2))
    {
        // Fetch a new word from memory using GetWord when offset crosses word boundaries
        new_word = GetWord((uint8_t *) entry_ptr, 4u * ((offset - offset2) / 4u + 1u));

        // A bit of magic calculations
        instr = (new_word >> (24u - ((offset - offset2) % 4u) * 8u)) & 0xffu;
    } else {
        // A bit of magic calculations
        instr = (new_word >> (24u - ((offset + offset2) % 4u) * 8u)) & 0xffu;
    }

    return instr;
}

/**
 * @fn          GetExidxEntry(const uint8_t* const section, const uint32_t offset)
 * @brief       This function decodes an entry in the Exidx (Exception Index) Table.
 * @param[in]   section
 * @param[in]   offset
 * @return      The exidx entry in both raw and decoded forms (exidxEntry_t)
 * 
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static exidxEntry_t ATTR_PURE GetExidxEntry(const uint8_t* const section, const uint32_t offset)
{
    exidxEntry_t entry = {0};
    entry.exidx_fn = GetWord(section, offset);
    entry.exidx_entry = GetWord(section, offset + 4u);

    // (Section 6)
    // The first word contains a prel31 offset (see Relocations) to the start of a function, with bit 31 clear.
    
    // Here, the function is decoded
    if ((entry.exidx_fn & COMPACT_MODEL_BITMASK) != COMPACT_MODEL_BITMASK)
    {
        entry.decoded_fn = DecodePrel31(entry.exidx_fn, (uint32_t) section + offset);
    }
    else
    {
        entry.decoded_fn = 0u;
    }

    // Here, the entry is decoded
    if ((entry.exidx_entry & COMPACT_MODEL_BITMASK) != COMPACT_MODEL_BITMASK)
    {
        entry.decoded_entry = DecodePrel31(entry.exidx_entry, (uint32_t) section + offset + 4u);
    }
    else
    {
        entry.decoded_entry = entry.exidx_entry;
    }

    return entry;
}

/**
 * @fn          DecodePrel31(const uint32_t word, const uint32_t where)
 * @brief       This decodes an offset with prel31 encoding.
 * @param[in]   word
 * @param[in]   where
 * @return      The prel31 offset of the word given in parameter
 * 
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE DecodePrel31(const uint32_t word, const uint32_t where)
{
    // Get the 31 weak bits of the word
    uint32_t offset = word & 0x7fffffffu;

    // Use the 31st bit as sign bit
    if ((offset & 0x40000000u) != 0u)
    {
        offset |= ~0x7fffffffu;
    }

    // Add the relocation
    offset += where;

    return offset;
}

/**
 * @fn          GetWord(const uint8_t* const section, const uint32_t offset)
 * @brief       This gets a word in a given offset of the section in parameter.
 * @param[in]   section Section from which the word will be taken
 * @param[in]   offset Offset in the section at which the word will be taken
 * @return      The 32-bit word at the specified offset.
 * 
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE GetWord(const uint8_t* const section, const uint32_t offset)
{
    return (
        (section[offset])
        | (section[offset + 1u] << 8)
        | (section[offset + 2u] << 16)
        | (section[offset + 3u] << 24)
    );
}
