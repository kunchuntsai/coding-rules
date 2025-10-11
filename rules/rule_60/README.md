# Rule 60: Avoid allocating and deallocating memory in different modules

## Explanation
Memory allocated in one module (DLL, shared library, or translation unit) must be deallocated in the same module. Different modules may use different heaps, different memory allocators, or different versions of the C runtime library. Crossing module boundaries with memory operations leads to undefined behavior, crashes, or memory corruption.

A "module" in this context typically refers to:
- Different DLLs/shared libraries (.dll, .so, .dylib)
- Different executables
- Code compiled with different compiler versions or settings
- Code using different C runtime libraries

## Pros of Following This Rule
- **Stability**: Prevents crashes from heap mismatches
- **Portability**: Works across different platforms and compilers
- **Maintainability**: Clear ownership of memory resources
- **No Memory Corruption**: Avoids undefined behavior from allocator mismatches

## Cons/Challenges
- **More Complex APIs**: Need factory/destructor functions or resource managers
- **Less Convenient**: Can't use simple new/delete across boundaries
- **Verbose Code**: Requires explicit resource management patterns
- **Performance**: May require extra function calls for allocation/deallocation

## Common Patterns to Follow This Rule

### Pattern 1: Factory and Destroyer Functions
```cpp
// Module API
MyObject* createObject();
void destroyObject(MyObject* obj);
```

### Pattern 2: Opaque Pointers with Module-Local Memory
```cpp
// Only the module that allocates manages the memory
class MyClass {
    void* internalData;  // Allocated and freed within same module
};
```

### Pattern 3: RAII Wrapper
```cpp
// Smart pointer that ensures same-module deallocation
class ModulePtr {
    void* ptr;
    void (*deleter)(void*);
};
```

### Pattern 4: Preallocated Buffers
```cpp
// Caller provides buffer, module just uses it
void processData(char* buffer, size_t size);
```

## Examples
See the example files in this directory for concrete demonstrations of correct and incorrect approaches.
