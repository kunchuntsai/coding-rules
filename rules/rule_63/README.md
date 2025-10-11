# Rule 63: Use sufficiently portable types in a module's interface

## Explanation
Module interfaces (DLL/shared library APIs) should use types that are portable and have stable, well-defined sizes across different platforms, compilers, and build configurations. Types like `int`, `long`, `size_t`, and `bool` can vary in size or representation across different environments, leading to ABI (Application Binary Interface) incompatibilities and crashes.

**Problem types (avoid in module interfaces):**
- `size_t` - varies between 32-bit and 64-bit systems
- `long` - 32-bit on Windows 64-bit, 64-bit on Linux 64-bit
- `bool` - size is implementation-defined (usually 1 byte, but not guaranteed)
- `wchar_t` - size varies by platform (2 bytes on Windows, 4 bytes on Linux)
- Platform-specific types

**Safe types (use in module interfaces):**
- Fixed-width integers: `int32_t`, `uint32_t`, `int64_t`, `uint64_t`
- `char` for bytes (always 1 byte)
- `float`, `double` (IEEE 754 standard sizes)
- Pointers to opaque types
- C-compatible struct layouts

## Pros of Following This Rule
- **ABI Stability**: Works across different platforms and architectures
- **No Surprises**: Explicit sizes prevent mismatches
- **Cross-Platform**: Same behavior on Windows, Linux, macOS
- **Future-Proof**: Resilient to compiler and platform changes

## Cons/Challenges
- **Less Convenient**: Can't use standard library types directly
- **More Verbose**: Need to specify exact types
- **Type Conversions**: May need to convert between internal and external types
- **C++11 Requirement**: Fixed-width types require `<cstdint>`

## Common Patterns

### Pattern 1: Fixed-Width Integers
```cpp
#include <cstdint>

// Good: Explicit sizes
int32_t getValue();
void setValue(uint64_t value);
```

### Pattern 2: C-Compatible Interface
```cpp
extern "C" {
    int32_t module_init(void);
    int32_t module_process(const char* input, uint32_t length);
}
```

### Pattern 3: Opaque Handles
```cpp
typedef struct OpaqueHandle* Handle;  // Pointer only, size is stable
Handle createHandle();
void destroyHandle(Handle h);
```

## Examples
See the example files in this directory for concrete demonstrations.
