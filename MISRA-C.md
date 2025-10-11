# Mapping C++ Coding Standards (Rules 57-63) to MISRA-C:2012

This document provides a comprehensive mapping between C++ Coding Standards Rules 57-63 (from "C++ Coding Standards" by Herb Sutter and Andrei Alexandrescu, 2005) and MISRA-C:2012 rules and directives.

## Quick Reference Mapping Table

| C++ Rule | MISRA-C:2012 Rule(s) | Relationship | Notes |
|----------|---------------------|--------------|-------|
| **57**: Keep type and interface in same namespace | N/A | No direct equivalent | C lacks namespaces; organize via file structure and naming conventions |
| **58**: Keep types and functions in separate namespaces | N/A | No direct equivalent | C lacks namespaces; use separate translation units and headers |
| **59**: `#include` before `using` | **Dir 4.10** (Required)<br>**Rule 20.1** (Advisory) | Partial match | Dir 4.10: Precautions for header inclusion<br>Rule 20.1: `#include` directives should only be preceded by preprocessor directives or comments |
| **60**: Avoid allocating/deallocating memory in different modules | **Dir 4.12** (Required)<br>**Rule 21.3** (Required) | Strong match | Dir 4.12: Dynamic memory allocation shall not be used<br>Rule 21.3: malloc/calloc/realloc/free functions shall not be used |
| **61**: Don't define entities with linkage in headers | **Rule 8.4** (Required)<br>**Rule 8.5** (Required)<br>**Rule 8.6** (Required) | Strong match | Rule 8.4: Compatible declaration visible for external linkage<br>Rule 8.5: External object/function declared once in one file<br>Rule 8.6: Identifier with external linkage has exactly one definition |
| **62**: Don't propagate exceptions across module boundaries | **Dir 4.7** (Required) | Conceptual match | C has no exceptions; Dir 4.7: Error information shall be tested (similar goal: explicit error handling) |
| **63**: Use portable types in module interfaces | **Rule 8.1** (Required)<br>**Rule 8.2** (Required)<br>**Rule 8.3** (Required) | Strong match | Rule 8.1: Types explicitly specified<br>Rule 8.2: Function types in prototype form<br>Rule 8.3: Declarations use same names and type qualifiers |

---

## Detailed MISRA-C:2012 Rule Descriptions

### Directives

**Directive 4.7** (Required): "If a function returns error information, then that error information shall be tested"
- **Related to C++ Rule 62**: Don't propagate exceptions across module boundaries
- **Rationale**: Since C has no exception handling, error information must be explicitly returned and checked
- **Application**: All functions returning error codes must have their return values checked
- **Example**:
  ```c
  // Good: Error code is checked
  int result = processData(input);
  if (result != SUCCESS) {
      handleError(result);
  }

  // Bad: Error code ignored
  processData(input);  // Violation of Dir 4.7
  ```

**Directive 4.10** (Required): "Precautions shall be taken in order to prevent the contents of a header file being included more than once"
- **Related to C++ Rule 59**: `#include` before `using`
- **Rationale**: Prevents multiple inclusion issues
- **Application**: Use include guards or `#pragma once`
- **Example**:
  ```c
  // Good: Include guard
  #ifndef MODULE_H
  #define MODULE_H

  // ... header contents ...

  #endif /* MODULE_H */
  ```

**Directive 4.12** (Required): "Dynamic memory allocation shall not be used"
- **Related to C++ Rule 60**: Avoid allocating/deallocating memory in different modules
- **Rationale**: MISRA-C takes a stricter approach - bans dynamic allocation entirely for safety-critical systems
- **Application**: No use of malloc, calloc, realloc, free
- **Difference from C++ Rule 60**: C++ allows dynamic allocation if managed within the same module; MISRA-C prohibits it entirely

---

### Rule 8 Series: Declarations and Definitions

**Rule 8.1** (Required): "Types shall be explicitly specified"
- **Related to C++ Rule 63**: Use portable types in module interfaces
- **Rationale**: Implicit types (e.g., implicit int) reduce portability and clarity
- **Example**:
  ```c
  // Bad: Implicit int
  static counter;  // Violation

  // Good: Explicit type
  static int counter;
  ```

**Rule 8.2** (Required): "Function types shall be in prototype form with named parameters"
- **Related to C++ Rule 63**: Use portable types in module interfaces
- **Rationale**: Ensures type safety and documents parameter purpose
- **Example**:
  ```c
  // Bad: Old-style declaration
  int process();  // Violation

  // Good: Prototype with named parameters
  int process(const char* data, size_t length);
  ```

**Rule 8.3** (Required): "All declarations of an object or function shall use the same names and type qualifiers"
- **Related to C++ Rules 61, 63**: Header definitions and portable types
- **Rationale**: Prevents inconsistencies across translation units
- **Example**:
  ```c
  // header.h
  extern int getValue(void);

  // source.c - Bad: Different return type
  long getValue(void) { return 42; }  // Violation

  // source.c - Good: Matches declaration
  int getValue(void) { return 42; }
  ```

**Rule 8.4** (Required): "A compatible declaration shall be visible when an object or function with external linkage is defined"
- **Related to C++ Rule 61**: Don't define entities with linkage in headers
- **Rationale**: Ensures declarations are visible before definitions, catches type mismatches
- **Example**:
  ```c
  // Bad: No declaration visible
  // source.c
  int globalVar = 42;  // Violation if no extern declaration in header

  // Good: Declaration in header
  // header.h
  extern int globalVar;
  // source.c
  int globalVar = 42;
  ```

**Rule 8.5** (Required): "An external object or function shall be declared once in one and only one file"
- **Related to C++ Rule 61**: Don't define entities with linkage in headers
- **Rationale**: Single point of declaration prevents inconsistencies
- **Example**:
  ```c
  // Bad: Declared in multiple headers
  // header1.h
  extern int sharedValue;
  // header2.h
  extern int sharedValue;  // Violation

  // Good: Declared in single header
  // common.h
  extern int sharedValue;
  ```

**Rule 8.6** (Required): "An identifier with external linkage shall have exactly one external definition"
- **Related to C++ Rule 61**: Don't define entities with linkage in headers (ODR)
- **Rationale**: Enforces One Definition Rule
- **Example**:
  ```c
  // Bad: Multiple definitions
  // file1.c
  int globalCounter = 0;
  // file2.c
  int globalCounter = 0;  // Violation: linker error

  // Good: Single definition
  // header.h
  extern int globalCounter;
  // file1.c
  int globalCounter = 0;  // Only definition
  ```

**Rule 8.7** (Advisory): "Functions and objects should not be defined with external linkage if they are referenced in only one translation unit"
- **Related to C++ Rules 58, 61**: Namespace organization and header definitions
- **Rationale**: Minimize scope, reduce unnecessary exposure
- **Example**:
  ```c
  // Bad: External linkage for local use
  int helperFunction(int x) { return x * 2; }  // Used only in this file

  // Good: Internal linkage
  static int helperFunction(int x) { return x * 2; }
  ```

**Rule 8.8** (Required): "The static storage class specifier shall be used in all declarations of objects and functions that have internal linkage"
- **Related to C++ Rules 58, 61**: Namespace organization and linkage control
- **Rationale**: Explicit linkage specification prevents ambiguity
- **Example**:
  ```c
  // Bad: Implicit internal linkage for const
  const int MAX_SIZE = 100;  // Violation (should be static const)

  // Good: Explicit static
  static const int MAX_SIZE = 100;
  ```

**Rule 8.9** (Advisory): "An object should be defined at block scope if its identifier only appears in a single function"
- **Related to C++ Rules 57-58**: Organizational principles
- **Rationale**: Minimize scope, improve encapsulation
- **Example**:
  ```c
  // Bad: File scope when only used in one function
  static int counter = 0;

  void increment(void) {
      counter++;
  }

  // Good: Block scope
  void increment(void) {
      static int counter = 0;
      counter++;
  }
  ```

**Rule 8.10** (Required): "An inline function shall be declared with the static storage class"
- **Related to C++ Rule 61**: Inline function handling in headers
- **Rationale**: Prevents multiple definition issues
- **Example**:
  ```c
  // Bad: Inline without static
  inline int square(int x) { return x * x; }  // Violation

  // Good: static inline
  static inline int square(int x) { return x * x; }
  ```

**Rule 8.11** (Required): "When an array with external linkage is declared, its size should be explicitly specified"
- **Related to C++ Rule 63**: Portable types in interfaces
- **Rationale**: Explicit sizes improve portability and safety
- **Example**:
  ```c
  // Bad: Size not specified
  extern int values[];  // Violation

  // Good: Explicit size
  extern int values[100];
  ```

**Rule 8.13** (Advisory): "A pointer should point to a const-qualified type whenever possible"
- **Related to C++ Rule 63**: Interface design and type safety
- **Rationale**: Const-correctness prevents unintended modifications
- **Example**:
  ```c
  // Bad: Modifiable pointer when not needed
  void printString(char* str);

  // Good: const pointer
  void printString(const char* str);
  ```

---

### Rule 11 Series: Conversions

**Rule 11.3** (Required): "A cast shall not be performed between a pointer to object type and a pointer to a different object type"
- **Related to C++ Rule 63**: Type safety in interfaces
- **Rationale**: Type punning can cause undefined behavior, alignment issues
- **Example**:
  ```c
  // Bad: Unsafe cast
  int* ptr = (int*)charArray;  // Violation

  // Good: Use union or proper type
  union {
      char bytes[4];
      int value;
  } converter;
  ```

**Rule 11.8** (Required): "A cast shall not remove any const or volatile qualification from the type pointed to by a pointer"
- **Related to C++ Rule 63**: Type safety
- **Example**:
  ```c
  // Bad: Removing const
  const int* cp = &value;
  int* p = (int*)cp;  // Violation

  // Good: Maintain const
  const int* cp = &value;
  const int* p = cp;
  ```

---

### Rule 20-21 Series: Preprocessing and Standard Library

**Rule 20.1** (Advisory): "`#include` directives should only be preceded by preprocessor directives or comments"
- **Related to C++ Rule 59**: `#include` before `using`
- **Rationale**: Ensures headers are included in clean context
- **Example**:
  ```c
  // Bad: Code before include
  #define MAX 100
  int dummy;  // Violation: non-preprocessor before include
  #include <stdio.h>

  // Good: Only preprocessor directives before include
  #define MAX 100
  #include <stdio.h>
  ```

**Rule 21.3** (Required): "The memory allocation and deallocation functions of `<stdlib.h>` shall not be used"
- **Related to C++ Rule 60**: Memory management across modules
- **Rationale**: Eliminates heap allocation issues entirely
- **Application**: No malloc, calloc, realloc, free
- **Example**:
  ```c
  // Bad: Dynamic allocation
  char* buffer = malloc(1024);  // Violation
  free(buffer);

  // Good: Static allocation
  char buffer[1024];
  ```

---

## Key Differences Between C++ Standards and MISRA-C:2012

### 1. Namespaces
- **C++ Rules 57-58**: Utilize namespaces for organization
- **MISRA-C**: No equivalent (C lacks namespaces)
- **C Alternative**: Use file structure, static functions, and naming conventions (e.g., `module_functionName`)

### 2. Memory Management
- **C++ Rule 60**: Allow dynamic allocation if managed within same module
- **MISRA-C Dir 4.12, Rule 21.3**: Ban dynamic allocation entirely
- **Rationale**: MISRA-C targets safety-critical systems where deterministic memory usage is required

### 3. Exception Handling
- **C++ Rule 62**: Catch exceptions at module boundaries, translate to error codes
- **MISRA-C Dir 4.7**: Error codes must be tested (C has no exceptions)
- **Both**: Emphasize explicit error handling across module boundaries

### 4. Linkage Rules (One Definition Rule)
- **C++ Rule 61**: Don't define entities with linkage in headers
- **MISRA-C Rules 8.4-8.6**: Very similar - enforce ODR strictly
- **Strong alignment**: Both prevent multiple definitions and ensure single declaration points

### 5. Type Safety
- **C++ Rule 63**: Use portable types in module interfaces
- **MISRA-C Rules 8.1-8.3**: Explicit types, prototypes, consistent declarations
- **Strong alignment**: Both emphasize explicit, portable type specifications

---

## Strictness Comparison

| Aspect | C++ Coding Standards | MISRA-C:2012 | Winner (Strictness) |
|--------|---------------------|--------------|---------------------|
| Dynamic Memory | Allowed within module | Completely banned | MISRA-C (stricter) |
| Type Specification | Recommended portable types | Mandatory explicit types | MISRA-C (stricter) |
| Error Handling | Use error codes at boundaries | Error codes must be tested | MISRA-C (stricter) |
| Linkage Control | Advisory (good practice) | Required/Mandatory | MISRA-C (stricter) |
| Header Includes | Good practice order | Specific ordering rules | Similar |
| Inline Functions | Can be in headers | Must be static inline | MISRA-C (stricter) |

---

## Practical Application Guide

### For C++ Developers Working with MISRA-C

If you're familiar with C++ Coding Standards Rules 57-63 and need to apply MISRA-C:2012:

1. **Rule 57-58 (Namespaces)**:
   - Replace with naming conventions: `module_function()`, `module_type_t`
   - Use separate translation units and headers
   - Apply Rule 8.7 (minimize external linkage)

2. **Rule 59 (Include Order)**:
   - Apply Dir 4.10 (include guards)
   - Apply Rule 20.1 (include order)

3. **Rule 60 (Memory Management)**:
   - Follow Dir 4.12 (no dynamic allocation at all)
   - If dynamic allocation is unavoidable, document deviation

4. **Rule 61 (Header Definitions)**:
   - Strictly apply Rules 8.4, 8.5, 8.6
   - Use `static inline` for inline functions (Rule 8.10)
   - Declare in header, define in `.c` file

5. **Rule 62 (Exceptions)**:
   - Use error codes instead
   - Apply Dir 4.7 (test all error returns)

6. **Rule 63 (Portable Types)**:
   - Apply Rules 8.1, 8.2, 8.3 strictly
   - Use `<stdint.h>` types (`int32_t`, `uint8_t`, etc.)
   - Always use function prototypes

### For MISRA-C Developers Learning C++

If you're familiar with MISRA-C and learning C++ standards:

1. **MISRA-C Rules 8.4-8.6 → C++ Rule 61**:
   - Same ODR principles apply
   - Templates and inline functions have special rules
   - Headers can contain: inline functions, templates, constexpr

2. **MISRA-C Dir 4.12, Rule 21.3 → C++ Rule 60**:
   - C++ allows dynamic memory (new/delete, smart pointers)
   - Critical: Allocate and deallocate in same module
   - Use RAII for automatic cleanup

3. **MISRA-C Dir 4.7 → C++ Rule 62**:
   - C++ has exceptions for internal use
   - At module boundaries: catch all, return error codes
   - Mark boundary functions `noexcept`

4. **MISRA-C Rules 8.1-8.3 → C++ Rule 63**:
   - Same principles: explicit, portable types
   - Use `<cstdint>` for fixed-width types
   - Avoid `size_t`, `long` at module boundaries

---

## Example: Module Interface Design

### MISRA-C:2012 Compliant Interface

```c
// module.h
#ifndef MODULE_H
#define MODULE_H

#include <stdint.h>

// Error codes (Dir 4.7)
typedef enum {
    MODULE_SUCCESS = 0,
    MODULE_ERR_INVALID_INPUT = -1,
    MODULE_ERR_NOMEM = -2
} module_error_t;

// Portable types (Rules 8.1, 8.2, 8.3)
int32_t module_initialize(void);
module_error_t module_process(const char* data, uint32_t length);
void module_cleanup(void);

#endif /* MODULE_H */
```

```c
// module.c
#include "module.h"

// Internal linkage (Rule 8.7, 8.8)
static int32_t internalCounter = 0;

// Definition matches declaration (Rules 8.4, 8.6)
int32_t module_initialize(void) {
    internalCounter = 0;
    return MODULE_SUCCESS;
}

module_error_t module_process(const char* data, uint32_t length) {
    // Check error (Dir 4.7 - caller must check our return value)
    if (data == NULL || length == 0) {
        return MODULE_ERR_INVALID_INPUT;
    }

    // No dynamic allocation (Dir 4.12, Rule 21.3)
    char buffer[256];  // Static allocation

    // Process data...
    internalCounter++;

    return MODULE_SUCCESS;
}

void module_cleanup(void) {
    internalCounter = 0;
    // No free() needed (no malloc used)
}
```

### C++ Equivalent Following Rules 60-63

```cpp
// module.h
#ifndef MODULE_H
#define MODULE_H

#include <cstdint>

namespace module {

// Error codes for module boundary (Rule 62)
enum class ErrorCode : int32_t {
    SUCCESS = 0,
    INVALID_INPUT = -1,
    OUT_OF_MEMORY = -2
};

// Portable types in interface (Rule 63)
ErrorCode initialize() noexcept;
ErrorCode process(const char* data, uint32_t length) noexcept;
void cleanup() noexcept;

// Internal implementation (Rule 60: memory managed internally)
class BufferManager {
public:
    BufferManager(size_t size);
    ~BufferManager();  // RAII: deallocates in same module
    char* data();
private:
    char* buffer_;
    size_t size_;
};

}  // namespace module

#endif
```

```cpp
// module.cpp
#include "module.h"
#include <stdexcept>

namespace module {

// Internal linkage (anonymous namespace)
namespace {
    int32_t internalCounter = 0;
}

// Rule 62: Catch exceptions at module boundary
ErrorCode initialize() noexcept {
    try {
        internalCounter = 0;
        return ErrorCode::SUCCESS;
    } catch (const std::bad_alloc&) {
        return ErrorCode::OUT_OF_MEMORY;
    } catch (...) {
        return ErrorCode::INVALID_INPUT;
    }
}

ErrorCode process(const char* data, uint32_t length) noexcept {
    try {
        if (!data || length == 0) {
            return ErrorCode::INVALID_INPUT;
        }

        // Rule 60: Allocation/deallocation in same module
        BufferManager buffer(256);

        // Process data...
        internalCounter++;

        return ErrorCode::SUCCESS;
    } catch (const std::bad_alloc&) {
        return ErrorCode::OUT_OF_MEMORY;
    } catch (...) {
        return ErrorCode::INVALID_INPUT;
    }
}

void cleanup() noexcept {
    internalCounter = 0;
}

// Rule 60: RAII ensures same-module deallocation
BufferManager::BufferManager(size_t size) : size_(size) {
    buffer_ = new char[size];  // Allocated in this module
}

BufferManager::~BufferManager() {
    delete[] buffer_;  // Deallocated in this module
}

char* BufferManager::data() {
    return buffer_;
}

}  // namespace module
```

---

## Summary

### Strong Matches (Direct Application)
- **C++ Rule 60 ↔ MISRA-C Dir 4.12, Rule 21.3**: Memory management (MISRA is stricter)
- **C++ Rule 61 ↔ MISRA-C Rules 8.4-8.6**: ODR and linkage control
- **C++ Rule 63 ↔ MISRA-C Rules 8.1-8.3**: Portable, explicit types

### Conceptual Matches (Similar Goals)
- **C++ Rule 62 ↔ MISRA-C Dir 4.7**: Error handling across boundaries
- **C++ Rule 59 ↔ MISRA-C Dir 4.10, Rule 20.1**: Header organization

### No Equivalent (Language Differences)
- **C++ Rules 57-58 ↔ MISRA-C**: N/A (namespaces don't exist in C)

### Overall Philosophy Alignment
Both C++ Coding Standards (Rules 57-63) and MISRA-C:2012 share core principles:
- **Explicit over implicit**: Clear types, declarations, linkage
- **Module boundary safety**: Careful interface design
- **Predictable behavior**: Avoid undefined behavior and hidden dependencies
- **Portability**: Platform-independent code

MISRA-C tends to be **more prescriptive and restrictive** (mandatory rules vs. guidelines), reflecting its safety-critical domain focus.

---

## References

- **C++ Coding Standards**: "101 Rules, Guidelines, and Best Practices" by Herb Sutter and Andrei Alexandrescu (2005)
- **MISRA-C:2012**: "Guidelines for the use of the C language in critical systems" (ISBN 978-1-906400-10-1)
- **MISRA-C:2012 Amendment 3**: Updates for ISO/IEC 9899:2011/2018
- **MathWorks MISRA-C Reference**: https://www.mathworks.com/help/bugfinder/misra-c-2012-reference.html
- **MISRA Official**: https://www.misra.org.uk/

---

## License

This mapping document is for educational purposes, demonstrating the relationship between C++ Coding Standards and MISRA-C:2012 rules. MISRA-C rules, text, and examples are copyrighted by the MISRA Consortium Limited. For official MISRA-C documentation, please visit https://www.misra.org.uk/.
