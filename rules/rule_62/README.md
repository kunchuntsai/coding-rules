# Rule 62: Don't allow exceptions to propagate across module boundaries

## Explanation
Exceptions should never propagate across module (DLL/shared library) boundaries. Different modules may be compiled with different compilers, different exception handling implementations, or different runtime libraries. When an exception crosses a module boundary, it can cause crashes, undefined behavior, or memory corruption.

A "module boundary" typically refers to:
- DLL/shared library interfaces (.dll, .so, .dylib)
- Plugin architectures
- Code compiled with different compilers
- Code using different C++ runtime libraries

**Safe approaches:**
- Catch all exceptions at module boundaries
- Convert exceptions to error codes
- Use `noexcept` on module interface functions
- Provide C-style error handling across boundaries

## Pros of Following This Rule
- **Stability**: Prevents crashes from exception ABI mismatches
- **Portability**: Works across different compilers and platforms
- **Clear Error Handling**: Explicit error codes are easier to understand
- **C Compatibility**: Can expose interfaces to C code

## Cons/Challenges
- **Less Convenient**: Can't use exception handling across boundaries
- **More Verbose**: Need to convert exceptions to error codes
- **Error Checking**: Callers must check return codes explicitly
- **Lost Information**: Stack traces and exception details may be lost

## Common Patterns

### Pattern 1: Convert Exceptions to Error Codes
```cpp
// Module interface
enum ErrorCode { SUCCESS, ERROR_INVALID_INPUT, ERROR_PROCESSING };

ErrorCode processData(const char* input) noexcept {
    try {
        // Internal processing that might throw
        return SUCCESS;
    } catch (const std::exception&) {
        return ERROR_PROCESSING;
    }
}
```

### Pattern 2: Output Parameter for Errors
```cpp
bool getData(int* out, ErrorCode* error) noexcept;
```

### Pattern 3: C Interface
```cpp
extern "C" {
    int module_function(void* data);  // Returns error code
}
```

## Examples
See the example files in this directory for concrete demonstrations.
