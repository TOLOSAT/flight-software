/**
 * @file    stacktrace.c
 * @author  Théo Bessel
 * @brief   Interface for stack trace handling
 * @note    Based on "Exception Handling ABI for the Arm Architecture" (6 october 2023)
 *
 * @copyright Copyright (c) TOLOSAT 2025
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
#define LR_STOP_UNWIND                 0xffffffffu /**< Last LR to which it is possible to unwind knowing that the signature of an exception return  */
#define FP_STOP_UNWIND                 0x07070707u /**< Last FP to which it is possible to unwind knowing that a task stack is initialised with r7 = 0x07070707u */

// ARM EXIDX entry specific constant
#define EXIDX_ENTRY_CANT_UNWIND        0x1u        /**< EXIDX entry value when unwinding is not possible */
#define EXIDX_ENTRY_COMPACT_MODEL_MASK 0x80000000u /**< EXIDX entry mask for compact model bit */
#define EXIDX_ENTRY_COMPACT_MODEL_POS  31u         /**< EXIDX entry position for compact model bit */
#define EXIDX_ENTRY_INDEX_MASK         0x0F000000u /**< EXIDX entry mask for index bits (indicates which personality routine is used) */
#define EXIDX_ENTRY_INDEX_MODEL_POS    24u         /**< EXIDX entry position for index bits (indicates which personality routine is used) */
#define SU16                           0x0u        /**< EXIDX entry SU16 personality routine index */
#define LU16                           0x1u        /**< EXIDX entry LU16 personality routine index */
#define LU32                           0x2u        /**< EXIDX entry LU32 personality routine index */

// PREL31 specific constant
#define PREL31_MASK                    0x7fffffffu /**< PREL31 Mask to get the 31 LSB bits */
#define PREL31_SIGN_BIT                0x40000000u /**< PREL31 sign bit */
#define PREL31_SIGN_EXTEND             0x80000000  /**< PREL31 sign extension */

// EXC_RETURN specific constant
#define EXC_RETURN_MASK                0xffffffe1u /**< EXC_RETURN_MASK to check if LR is an EXC_RETURN code */

/**
 * @def     GET_INSTR_6LSB(instruction)
 * @brief   Preprocessor function that gets the 6 LSBs of an instruction
 */
#define GET_INSTR_6LSB(instruction)    (((instruction) & 0x3fu) << 2) /**< Mask for getting the 6 LSB */

/**
 * @def     LAST_CALL(call_stack)
 * @brief   Preprocessor function that gets the last call in the stack trace
 */
#define LAST_CALL(call_stack)          ((call_stack)->calls[(call_stack)->last_idx])

/*************************** Functions Declarations **************************/

static void UnwindNextFrame(callStack_t *call_stack);

static uint32_t DecodeFrame(uint32_t entry, uint32_t decoded_entry, uint32_t fp);
static uint32_t DecodeCompactModelEntry(const uint32_t entry, const uint32_t word, const uint32_t fp, const uint32_t instr_count,
                                        const uint32_t offset);
static uint32_t GetInstruction(const uint32_t entry, const uint32_t word, const uint32_t offset, const uint32_t offset2);
static exidxEntry_t DecodeExidxEntry(const exidxEntry_t *const raw_entry);
static uint32_t DecodePrel31(const uint32_t *const prel31_ptr);

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
void UnwindStackFromContext(callStack_t *call_stack, call_t last_call)
{
    call_stack->last_idx = 0u;

    // Setup last call
    LAST_CALL(call_stack) = last_call;

    while ((call_stack->last_idx < (uint32_t)CONFIG_CALL_STACK_MAX_SIZE)        // Stop if reached the max capacity of the stack trace
           && ((LAST_CALL(call_stack).lr & EXC_RETURN_MASK) != EXC_RETURN_MASK) // Stop if the link register is an EXEC RETURN
           && (LAST_CALL(call_stack).lr != LR_STOP_UNWIND)                      // Stop if the start of a task stack has been reached
           && (LAST_CALL(call_stack).fp != FP_STOP_UNWIND))                     // Stop if the start of a task stack has been reached
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
static void UnwindNextFrame(callStack_t *call_stack)
{
    // Get exidx table and size
    exidxEntry_t *exidx_table = (exidxEntry_t *)&__exidx_start; // cppcheck-suppress misra-c2012-11.3; Exception: this is the only way to create a
                                                                // table for exidx, normally we dont have misalignement because __exidx_start is just
                                                                // a symbol to get the address
    uint32_t exidx_nb_entries = ((uint32_t)&__exidx_end - (uint32_t)&__exidx_start) / sizeof(exidxEntry_t); // cppcheck-suppress misra-c2012-11.4;
                                                                                                            // Exception: this is the only way to know
                                                                                                            // the section size and thus number of
                                                                                                            // entries

    // Total number of entries in the unwind table
    uint32_t entry_count = exidx_nb_entries;
    // Exidx table entry
    exidxEntry_t decoded_entry = { 0 };

    // Iterate over all entries, get the function return address and find the entry
    // corresponding to the last return address unwound (ie. the address of the function
    // associated with the frame to unwind).
    //
    // TO DO : Could be optimized with a dichotomic search because addresses are sorted in
    // unwind table. The complexity would then be O(log_2(N)) instead of O(N)
    do
    {
        entry_count--;
        decoded_entry = DecodeExidxEntry(&exidx_table[entry_count]);
        (void)(decoded_entry);
    } while ((entry_count > 0u) && (decoded_entry.exidx_fn > LAST_CALL(call_stack).lr));

    // Save current frame pointer (will be required to decode the next frame)
    uint32_t current_fp = LAST_CALL(call_stack).fp;

    // Update lr with the last function called (TO DO : improve by getting the exact instruction)
    LAST_CALL(call_stack).lr = decoded_entry.exidx_fn;

    // Move to the next call array place
    call_stack->last_idx++;

    // The second word contains one of:
    //   - The prel31 offset of the start of the table entry for this function, with bit 31 clear.
    //   - The exception-handling table entry itself with bit 31 set, if it can be encoded in 31 bits
    //     (see The Arm-defined compact model).
    //   - The special bit pattern EXIDX_ENTRY_CANT_UNWIND (0x1), indicating to run-time support code that associated
    //     frames cannot be unwound. On encountering this pattern the language-independent unwinding routines
    //     return a failure code to their caller, which should take an appropriate action such as calling
    //     terminate() or abort(). See Phase 1 unwinding and Phase 2 unwinding.
    if (exidx_table[entry_count].extab_entry == EXIDX_ENTRY_CANT_UNWIND) // Special pattern 0x1 EXIDX_ENTRY_CANT_UNWIND
    {
        LAST_CALL(call_stack).lr = LR_STOP_UNWIND;
        LAST_CALL(call_stack).fp = LR_STOP_UNWIND;
    }
    else if ((exidx_table[entry_count].extab_entry & EXIDX_ENTRY_COMPACT_MODEL_MASK) != 0u) // Bit 31 set --> compact model
    {
        // cppcheck-suppress misra-c2012-11.4; Exception: new_fp needs to be used as an array to get lr and fp
        uint32_t *new_fp = (uint32_t *)DecodeFrame(exidx_table[entry_count].extab_entry, decoded_entry.extab_entry, current_fp);

        /**
         * The `lr` register is pushed just before the `fp` register, then we can get it by accessing `fp + 4`
         */
        LAST_CALL(call_stack).fp = new_fp[0u];
        if ((new_fp[1u] & EXC_RETURN_MASK) == EXC_RETURN_MASK)
        {
            LAST_CALL(call_stack).lr = new_fp[1u];
        }
        else
        {
            LAST_CALL(call_stack).lr = new_fp[1u] - 1u;
        }
    }
    else // Bit 31 is clear
    {
        uint32_t extab_entry = *((uint32_t *)decoded_entry.extab_entry); // cppcheck-suppress misra-c2012-11.4; Exception: decoded_entry.extab_entry
                                                                         // points to the extab entry

        if ((extab_entry & EXIDX_ENTRY_COMPACT_MODEL_MASK) != 0u)
        {
            uint32_t *new_fp = (uint32_t *)DecodeFrame(extab_entry, decoded_entry.extab_entry, current_fp); // cppcheck-suppress misra-c2012-11.4;
                                                                                                            // Exception: new_fp needs to be used as
                                                                                                            // an array to get lr and fp

            /**
             * The `lr` register is pushed just before the `fp` register, then we can get it by accessing `fp + 4`
             */
            LAST_CALL(call_stack).fp = new_fp[0u];
            if ((new_fp[1u] & EXC_RETURN_MASK) == EXC_RETURN_MASK)
            {
                LAST_CALL(call_stack).lr = new_fp[1u];
            }
            else
            {
                LAST_CALL(call_stack).lr = new_fp[1u] - 1u;
            }
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
    uint32_t personnality_routine = (entry & EXIDX_ENTRY_INDEX_MASK) >> EXIDX_ENTRY_INDEX_MODEL_POS;
    switch (personnality_routine)
    {
        // (Section 10.2)
        // Short 3 unwinding instructions in bits 16-23, 8-15, and 0-7 of the first word. Any of the instructions can be Finish.
        case SU16 :
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 3u, 1u);
            break;

        // (Section 10.2)
        // Long Bits 16-23 contain a count N of the number of additional 4-byte words that contain unwinding instructions.
        // The sequence of unwinding instructions is packed into bits 8-15, 0-7, and the following N words.
        // Spare trailing bytes in the last word should be filled with Finish instructions.
        case LU16 :
        case LU32 :
            new_fp = DecodeCompactModelEntry(decoded_entry, word, fp, 2u + (4u * instr_count), 2u);
            break;
        default :
            // error
            break;
    }

    // Re-aligns the framepointer to 8 bits
    new_fp += (-new_fp) & 7u;

    return new_fp;
}

/**
 * @fn          DecodeCompactModelEntry(const uint32_t entry, const uint32_t word, const uint32_t fp, const uint32_t instr_count, const uint32_t
 * offset)
 * @brief       This function decodes unwind instructions based on ARM EHABI standard.
 * @param[in]   entry       The address of the words to decode
 * @param[in]   word        The original word decoded
 * @param[in]   fp          The old frame pointer
 * @param[in]   instr_count The number of instructions
 * @param[in]   offset      A specific offset within the word (= 1 or 2 depending of the compact model index)
 * @return      The new frame pointer
 *
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE DecodeCompactModelEntry(const uint32_t entry, const uint32_t word, const uint32_t fp, const uint32_t instr_count,
                                                  const uint32_t offset)
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
        uint32_t instr1 = GetInstruction(entry, word, instr_index, offset);
        uint32_t instr2 = 0u;
        if (double_instr)
        {
            instr2 = GetInstruction(entry, word, instr_index + 1u, offset);
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
        else if (double_instr && (instr1 == 0x80u) && (instr2 == 0x00u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 & 0xf0u) == 0x80u)
        {
            instr_index++;
        }
        else if ((instr1 == 0x9du))
        {
            __NOP();
        }
        else if ((instr1 == 0x9fu))
        {
            __NOP();
        }
        else if ((instr1 & 0xf0u) == 0x90u)
        {
            __NOP();
        }
        else if ((instr1 & 0xf8u) == 0xa0u)
        {
            __NOP();
        }
        else if ((instr1 & 0xf8u) == 0xa8u)
        {
            __NOP();
        }
        else if ((instr1 == 0xb0u))
        {
            __NOP();
        }
        else if (double_instr && (instr1 == 0xb1u) && (instr2 == 0x00u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xb1u) && ((instr2 & 0xf0u) == 0x00u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xb1u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xb2u))
        {
            // 10110010 uleb128
            // vsp = vsp + 0x204+ (uleb128 << 2) (for vsp increments of 0x104-0x200, use 00xxxxxx twice)
            new_fp += 0x204u + (instr2 << 2);
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xb3u))
        {
            instr_index++;
        }
        else if ((instr1 == 0xb4u))
        {
            __NOP();
        }
        else if ((instr1 & 0xf8u) == 0xb8u)
        {
            __NOP();
        }
        else if ((instr1 & 0xf8u) == 0xc0u)
        {
            __NOP();
        }
        else if (double_instr && (instr1 == 0xc6u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xc7u) && (instr2 == 0x00u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xc7u) && ((instr2 & 0xf0u) == 0x00u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xc7u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xc8u))
        {
            instr_index++;
        }
        else if (double_instr && (instr1 == 0xc9u))
        {
            instr_index++;
        }
        else if ((instr1 & 0xf8u) == 0xc8u)
        {
            __NOP();
        }
        else if ((instr1 & 0xf8u) == 0xd0u)
        {
            __NOP();
        }
        else if ((instr1 & 0xc0u) == 0xc0u)
        {
            __NOP();
        }
        else
        {
            __NOP();
        }

        instr_index++;
    }

    return new_fp;
}

/**
 * @fn          GetInstruction(const uint32_t entry, const uint32_t word, const uint32_t offset, const uint32_t offset2)
 * @brief       This function fetches an unwind instruction given in parameter.
 * @param[in]   entry       The address of the words to decode
 * @param[in]   word        The original word decoded
 * @param[in]   offset      The offset in the section
 * @param[in]   offset2     The offset in the word
 * @return      The instruction contained at address of entry_ptr with given offsets
 *
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE GetInstruction(const uint32_t entry, const uint32_t word, const uint32_t offset, const uint32_t offset2)
{
    uint32_t instr    = 0x0u;
    uint32_t new_word = word;

    // Calculate which word we need to access based on the offset
    if (offset >= (4u - offset2))
    {
        // Fetch a new word from memory
        new_word = ((uint32_t *)entry)[(offset - offset2) + 1u]; // cppcheck-suppress misra-c2012-11.4; Exception: new_word needs to be accessed from
                                                                 // entry as an array

        // A bit of magic calculations
        instr = (new_word >> (24u - ((offset - offset2) % 4u) * 8u)) & 0xffu;
    }
    else
    {
        // A bit of magic calculations
        instr = (new_word >> (24u - ((offset + offset2) % 4u) * 8u)) & 0xffu;
    }

    return instr;
}

/**
 * @fn          DecodeExidxEntry(const exidxEntry_t *const raw_entry)
 * @brief       This function decodes an entry in the Exidx (Exception Index) Table.
 * @param[in]   raw_entry   Raw exidx entry
 * @return      The decoded exidx entry
 *
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static exidxEntry_t ATTR_PURE DecodeExidxEntry(const exidxEntry_t *const raw_entry)
{
    exidxEntry_t decoded_entry = { 0 };

    // (Section 6)
    // The first word contains a prel31 offset (see Relocations) to the start of a function, with bit 31 clear.

    // Decoding the function
    if ((raw_entry->exidx_fn & EXIDX_ENTRY_COMPACT_MODEL_MASK) != EXIDX_ENTRY_COMPACT_MODEL_MASK)
    {
        decoded_entry.exidx_fn = DecodePrel31(&raw_entry->exidx_fn);
    }
    else
    {
        decoded_entry.exidx_fn = 0u;
    }

    // Decoding the extab entry
    if ((raw_entry->extab_entry & EXIDX_ENTRY_COMPACT_MODEL_MASK) != EXIDX_ENTRY_COMPACT_MODEL_MASK)
    {
        decoded_entry.extab_entry = DecodePrel31(&raw_entry->extab_entry);
    }
    else
    {
        decoded_entry.extab_entry = raw_entry->extab_entry;
    }

    return decoded_entry;
}

/**
 * @fn          DecodePrel31(const uint32_t *const prel31_ptr)
 * @brief       This decodes an prel31 pointer.
 * @param[in]   prel31_ptr Prel31 pointer to decode
 * @return      The decoded prel31 address
 *
 * @warning This function is annotated with the `pure` attribute for performance purpose, it must remain pure if it's changed
 */
static uint32_t ATTR_PURE DecodePrel31(const uint32_t *const prel31_ptr)
{
    // Prepare decoded address with prel31_addr
    uint32_t decoded_address = (uint32_t)prel31_ptr; // cppcheck-suppress misra-c2012-11.4; Exception: this is the only way to get the addres of
                                                     // prel31_ptr

    // Extract the 31-bit signed offset directly from the address content
    uint32_t offset = *prel31_ptr & PREL31_MASK;

    // Check if the offset is negative or not
    if ((offset & PREL31_SIGN_BIT) == PREL31_SIGN_BIT)
    {
        // Offset is negative
        offset           = ~(offset | PREL31_SIGN_EXTEND) + 1u; // Two's complements
        decoded_address -= offset;
    }
    else
    {
        // Offset is positive
        decoded_address += offset;
    }

    return decoded_address;
}
