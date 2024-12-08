/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @author  Theo Bessel
 * @brief   Source file with dummy tasks
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void FunctionA(uint32_t a);
static void FunctionB(uint32_t b);
static void FunctionC(uint32_t c);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask(void)
{
    // Initialisation
    LOG("Init dummy task\n");

    // Function Core
    while (1)
    {
        // Get time
        LOG("Hello\n");

        FunctionA(13u);

        SleepPeriodic();
    }
}

/**
 * @fn      FunctionA(uint32_t a)
 * @brief   Just dummy function to fill the stack trace
 */
static void FunctionA(uint32_t a)
{
    // Random operation to have frames with registers pushed on the stack
    LOG("Function A\n");
    LOG_DECIMAL("a=%d\n", a);

    // Calling FunctionB (which calls FunctionC that causes UsageFault)
    FunctionB((a - 10u));
}

/**
 * @fn      FunctionB(uint32_t b)
 * @brief   Just dummy function to fill the stack trace
 */
static void FunctionB(uint32_t b)
{
    // Random operation to have frames with registers pushed on the stack
    volatile uint32_t c = 32u - b;

    LOG("Function B\n");
    LOG_DECIMAL("b=%d\n", b);
    LOG_DECIMAL("c=%d\n", c);

    // Calling FunctionC (which causes UsageFault)
    FunctionC(c);
}

/**
 * @fn      FunctionC(uint32_t c)
 * @brief   Just dummy function to fill the stack trace
 */
static void FunctionC(uint32_t c)
{
    // Random operation to have frames with registers pushed on the stack
    volatile uint32_t a = c + 43u;
    volatile uint32_t b = 0u;

    LOG("Function C\n");
    LOG_DECIMAL("a=%d\n", a);
    LOG_DECIMAL("b=%d\n", b);

    // Causes UsageFault
    volatile uint32_t result = a / b; // cppcheck-suppress zerodiv; That's the point of this test
    (void)(result);
}
