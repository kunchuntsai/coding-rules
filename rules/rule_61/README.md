# Rule 61: Don't define entities with linkage in a header file

## Explanation
Header files should contain only **declarations**, not **definitions** of entities with external linkage. Definitions with linkage belong in implementation files (.cpp). If a header defines non-inline functions, non-inline variables, or other entities with external linkage, every translation unit that includes that header will create its own copy, violating the One Definition Rule (ODR) and causing linker errors or undefined behavior.

**Entities that CAN be defined in headers:**
- Inline functions (`inline` keyword)
- Templates (function templates, class templates)
- `constexpr` variables (implicitly inline in C++17+)
- Class definitions (but not their non-inline member function bodies)
- `inline` variables (C++17+)

**Entities that CANNOT be defined in headers:**
- Non-inline function definitions
- Non-inline global variables
- Non-inline static member variables (pre-C++17)

## Pros of Following This Rule
- **No ODR Violations**: Avoids multiple definition errors
- **Faster Compilation**: Headers remain lightweight
- **Proper Linkage**: Clear separation between interface and implementation
- **No Linker Errors**: Prevents "multiple definition" errors

## Cons/Challenges
- **Less Convenient**: Can't define everything in one place
- **Requires .cpp Files**: Need separate implementation files
- **Inline Consideration**: Must explicitly mark appropriate functions as inline

## Common Patterns

### Pattern 1: Declaration in Header, Definition in .cpp
```cpp
// header.h
extern int globalCounter;  // Declaration only
void processData();        // Declaration only

// header.cpp
int globalCounter = 0;     // Definition
void processData() { }     // Definition
```

### Pattern 2: Inline Functions (OK in Headers)
```cpp
// header.h
inline int add(int a, int b) {  // OK: inline
    return a + b;
}
```

### Pattern 3: Templates (OK in Headers)
```cpp
// header.h
template<typename T>
T max(T a, T b) {  // OK: template
    return (a > b) ? a : b;
}
```

## Examples
See the example files in this directory for concrete demonstrations.
