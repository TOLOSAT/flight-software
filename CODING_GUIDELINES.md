# Flight Software C Coding Guidelines

## 1. Purpose & Scope

* Centralise C coding practices for the flight software repository.
* Align with MISRA C and project templates while maintaining consistency across modules.
* Require every contributor to read this document before contributing code.

## 2. Structure & Style

### 2.1 File & Module Layout

* Each `.c` and `.h` file begins with a Doxygen banner containing the file name, a brief description, the author, and a copyright.
* Keep all standard section banners:
  `Include Files`, `Macros Definitions`, `Functions Declarations`, `Variables Definitions`, `Functions Definitions`, etc.
  Keep empty sections to preserve the structure.
* Implement public APIs in headers; define them in the corresponding source file.
* Protect headers with uppercase include guards (`KERNEL_H`, `FDIR_CONTEXT_H`, etc.).
* Internal helper functions must be declared `static` in the `.c` file; public functions declared `extern` in the header.
* Keep the **public API minimal** - expose only what is required externally.

### 2.2 Naming Conventions

| Category                | Convention              | Example                                  |
| ----------------------- | ----------------------- | ---------------------------------------- |
| Macros / Enum constants | FULLCAPS_SNAKE_CASE     | `#define CONFIG_MAX_TASKS 32u`           |
| Functions               | PascalCase              | `void InitBoard(void);`                  |
| Type aliases            | camelCase + `_t` suffix | `typedef struct { ... } deviceHandle_t;` |
| Variables               | snake_case              | `uint32_t task_count = 0u;`              |
| Globals                 | prefix with `g_`        | `uint32_t g_scheduler_tick = 0u;`        |
| Pointers                | prefix with `p_`        | `taskDesc_t *p_task_desc = NULL;`        |

Use descriptive names; avoid abbreviations without explanation.

### 2.3 Typing & Literals

* Use fixed-width types (`uint8_t`, `int32_t`, `float16_t`, ...) from `<stdint.h>` / `<stdfloat.h>`.
* Match literal suffixes to type (`0u`, `1ull`, `1.0f`, ...).
* Replace magic numbers with named constants or enumerations.
* Always initialise variables upon declaration.

### 2.4 Formatting Rules

* Apply the repository `.clang-format` profile (Allman style, 4-space indentation, column limit 150).
* Align assignments and macros for readability.
* Run the formatter before every commit; never manually re-indent code.
* Keep include lists clean and remove unused headers.

## 3. Documentation & Comments

### 3.1 Doxygen Standards

* **Files:** Start each file with a Doxygen header (`@file`, `@brief`, `@author`).
* **Functions:**

  * Place Doxygen block directly above definition.
  * Minimum tags: `@fn`, `@brief`, `@param`, `@return` (or `@retval`).
* **Variables:** Document global variables with `@var` and `@brief`; local `static` variables don't require Doxygen.
* **Types:** Document every `typedef`, `enum`, and `struct`.
  Each member should include an inline comment `/**< description */`.
* **Macros:** Add concise comments describing their intent and use.

### 3.2 Commenting Practices

* Write complete English sentences describing the **intent** of the logic, not just the behavior.
* Explain non-obvious or critical logic; skip trivial comments.
* Remove commented-out or obsolete code immediately.
* Use `/* ... */` for block comments and `//` only for short, local notes.

## 4. Coding Rules

### 4.1 Function & Control Flow

* Each function must have **a single return point**; accumulate the status in a local variable and return it at the end. ISO 26262 and IEC 61508 emphasise single-exit constructs because extra exits often skip required shutdown or cleanup logic, making dataflow auditing and qualification tools less reliable. Avoiding those gaps prevents unintended side effects such as leaving peripherals in an unsafe state, skipping unlock routines, or returning partially initialised data.
* Return `returnCode_t` for non-void functions; accepted codes include:
  `RETURN_CODE_SUCCESS`, `RETURN_CODE_ERROR`, `RETURN_CODE_INVALID_PARAM`, etc.
* Always check and handle return values from functions that may fail.
* Assign function results to a variable before using them in conditionals.

**Conditional order:** always branch from the most nominal scenario down to the broadest error case. This keeps the control flow linear for step-by-step debugging and makes deviations from the expected behaviour obvious.

```c
if (is_ready == true)
{
    retcode = StartTask(task_id);
}
else if (is_degraded == true)
{
    retcode = HandleDegradedMode(task_id);
}
else
{
    retcode = RETURN_CODE_ERROR;
}
```

**Forbidden constructs:** safety standards such as ISO 26262 and IEC 61508 flag these patterns because they often bypass required termination or cleanup logic and create hard-to-detect side effects (for example, peripherals left in unsafe states, locks never released, or partially initialised data exposed). Avoid:
* Multiple `return` statements or other early exits.
* `goto`, computed jumps, implicit `switch` fallthroughs.
* Premature `break`/`continue` in loops - use flags or loop conditions instead.
* Dead or unreachable code.

### 4.2 Platform Constraints

* **No dynamic memory allocation.** Use static or stack storage only.
* Explicitly initialise all buffers and aggregates before use.
* Avoid standard library calls except: `memcpy`, `memset`, and `strcmp`.

### 4.3 MISRA Compliance & Build Rules

* Maintain MISRA C:2012 compliance at all times.
* Any justified deviations must follow the project's deviation process and be referenced in code.
* Treat all compiler warnings as errors; fix before committing.
* Perform self-review for style and MISRA adherence before peer review.

### 4.4 Version Control & Git Workflow

* Prefix commits with verbs: `feat:`, `fix:`, `ref:`, `chore:`.
* Combine scopes if needed:
  `fix: missing doxygen && ref: split foo.c into foo.c and bar.c`.
* Commit small, coherent changes; keep history clean and linear.

## 5. Quick Reference

| Category      | Key Principle                                           |
| ------------- | ------------------------------------------------------- |
| Structure     | Template sections, static helpers, guarded headers      |
| Style         | Allman format, descriptive names, initialised variables |
| Documentation | Doxygen everywhere, comment intent not behavior         |
| Safety        | No malloc, MISRA compliant, handle all return codes     |
| Workflow      | Clean commits, minimal public API, formatted code       |
