# C++ Coding Standards: Namespaces and Modules
```
Sutter C++ Coding Standards. 2005
http://library.bagrintsev.me/CPP/Sutter.C%2B%2B%20Coding%20Standards.2005.pdf
```
This repository contains examples and explanations for Rules 57-63 from "C++ Coding Standards" by Herb Sutter and Andrei Alexandrescu (2005). These rules cover namespace organization (57-59) and module interface design (60-63).

## Summary Table

| Rule | Good For | Not Recommended For |
|------|----------|---------------------|
| **57** | Non-member functions as part of type's interface | Member functions, unrelated utilities |
| **58** | Organizing cohesive functionality | Mixing unrelated concerns |
| **59** | Always: `#include` before `using` | Never skip this rule |
| **60** | DLLs/shared libraries with separate heaps | Single executables with unified memory |
| **61** | Declarations, inline, templates in headers | Non-inline definitions in headers |
| **62** | DLL/shared library interfaces | Internal APIs within same binary |
| **63** | Portable types in module interfaces | Implementation-specific types |

## Table of Contents

- [Overview of Namespace and Module Rules](#overview-of-namespace-and-module-rules)
  - [Rule 57: Keep a type and its nonmember function interface in the same namespace](#rule-57-keep-a-type-and-its-nonmember-function-interface-in-the-same-namespace)
  - [Rule 58: Keep types and functions in separate namespaces unless they're specifically intended to work together](#rule-58-keep-types-and-functions-in-separate-namespaces-unless-theyre-specifically-intended-to-work-together)
  - [Rule 59: Don't write namespace using-directives or using-declarations before #include directives](#rule-59-dont-write-namespace-using-directives-or-using-declarations-before-include-directives)
  - [Rule 60: Avoid allocating and deallocating memory in different modules](#rule-60-avoid-allocating-and-deallocating-memory-in-different-modules)
  - [Rule 61: Don't define entities with linkage in a header file](#rule-61-dont-define-entities-with-linkage-in-a-header-file)
  - [Rule 62: Don't allow exceptions to propagate across module boundaries](#rule-62-dont-allow-exceptions-to-propagate-across-module-boundaries)
  - [Rule 63: Use sufficiently portable types in a module's interface](#rule-63-use-sufficiently-portable-types-in-a-modules-interface)
- [Quick Reference Chart](#quick-reference-chart)
- [Common Themes](#common-themes)
- [Resources](#resources)
- [License](#license)

## Overview of Namespace and Module Rules

### Rule 57: Keep a type and its nonmember function interface in the same namespace

**Summary**: Functions that are part of a type's interface but aren't member functions should be in the same namespace as the type.

**Reason**: Enables argument-dependent lookup (ADL) and keeps related functionality together. When non-member interface functions are in the same namespace, they can be found via ADL, making the code more intuitive and following the principle of least surprise.

**Example (good)**:
```cpp
namespace geometry {
    class Point {
    public:
        Point(double x, double y) : x_(x), y_(y) {}
        double x() const { return x_; }
        double y() const { return y_; }
    private:
        double x_, y_;
    };

    // Good: distance is part of Point's interface and in same namespace
    double distance(const Point& p1, const Point& p2) {
        double dx = p1.x() - p2.x();
        double dy = p1.y() - p2.y();
        return std::sqrt(dx*dx + dy*dy);
    }

    // Good: operator<< for Point is in same namespace
    std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x() << ", " << p.y() << ")";
    }
}

// Usage: ADL finds the function automatically
geometry::Point p1(0, 0), p2(3, 4);
double d = distance(p1, p2);  // ADL finds geometry::distance
std::cout << p1 << std::endl;  // ADL finds geometry::operator<<
```

**Example (bad)**:
```cpp
namespace geometry {
    class Point { /*...*/ };
}

// Bad: distance is separated from Point
namespace utility {
    double distance(const geometry::Point& p1, const geometry::Point& p2);
}

// Usage now requires explicit qualification
geometry::Point p1(0, 0), p2(3, 4);
double d = utility::distance(p1, p2);  // Must qualify, ADL won't find it
```

**Decision**:
- **Good for**: Non-member functions that logically extend a type's interface (operators, factory functions, algorithm functions)
  ```cpp
  namespace math {
      class Matrix { /*...*/ };
      Matrix operator*(const Matrix& a, const Matrix& b);  // Good: part of Matrix interface
      Matrix transpose(const Matrix& m);  // Good: logical extension
  }
  ```
- **Not recommended for**: Member functions (use member functions when they need access to private data), unrelated utility functions that just happen to use the type
  ```cpp
  namespace data {
      class Record { /*...*/ };

      // Bad: serialize is general utility, not part of Record's interface
      std::string serialize(const Record& r);
      // Better: put in separate utility namespace
  }

  namespace serialization {
      std::string serialize(const data::Record& r);  // Better location
  }
  ```

---

### Rule 58: Keep types and functions in separate namespaces unless they're specifically intended to work together

**Summary**: Don't mix unrelated functionality in the same namespace. Group related things together, separate independent concerns.

**Reason**: Reduces coupling, prevents naming conflicts, improves modularity, and makes dependencies explicit. Cohesive namespaces make code easier to understand and maintain.

**Example (good)**:
```cpp
namespace graphics {
    class Color { /*...*/ };
    class Shape { /*...*/ };
    void render(const Shape& s, const Color& c);
}

namespace io {
    class FileHandle { /*...*/ };
    class Reader { /*...*/ };
    void open(FileHandle& fh, const char* path);
}

namespace network {
    class Socket { /*...*/ };
    class Protocol { /*...*/ };
    void connect(Socket& s, const char* address);
}
// Good: Each namespace has cohesive, related functionality
```

**Example (bad)**:
```cpp
namespace myapp {
    // Bad: Mixing unrelated concerns
    class Color { /*...*/ };
    class FileHandle { /*...*/ };
    class Socket { /*...*/ };
    class DatabaseConnection { /*...*/ };
    class JsonParser { /*...*/ };
    // Everything dumped in one namespace!
}
```

**Decision**:
- **Good for**: Organizing cohesive functionality, creating logical module boundaries, managing dependencies
  ```cpp
  namespace database {
      class Connection { /*...*/ };
      class Query { /*...*/ };
      class Transaction { /*...*/ };
      // Good: all database-related, work together
  }
  ```
- **Not recommended for**: Mixing unrelated concerns just because they're in the same project, creating artificial separation when types work closely together
  ```cpp
  // Bad: Artificial over-separation
  namespace point { class Point { /*...*/ }; }
  namespace rectangle {
      class Rectangle {
          point::Point topLeft;  // Tedious!
          point::Point bottomRight;
      };
  }

  // Better: Keep closely related types together
  namespace geometry {
      class Point { /*...*/ };
      class Rectangle { /*...*/ };  // They work together
  }
  ```

---

### Rule 59: Don't write namespace using-directives or using-declarations before #include directives

**Summary**: Always place `#include` directives before any `using` statements.

**Reason**: Prevents headers from being interpreted in unexpected namespace contexts, avoiding subtle bugs. Headers should be included in a clean namespace state to ensure consistent behavior regardless of where they're included.

**Example (good)**:
```cpp
// Good: #include before using
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;

void processData(const string& data) {
    vector<int> values;
    cout << "Processing: " << data << "\n";
}
```

**Example (bad)**:
```cpp
// Bad: using before #include
using std::string;  // WRONG ORDER!
#include <iostream>  // This header is now interpreted with 'string' already in scope
#include <custom_header.h>  // May be affected by using declarations

// If custom_header.h has its own 'string' type, conflicts occur!
```

**Decision**:
- **Good for**: Always - this rule should never be violated
  ```cpp
  // Always do this:
  #include <vector>
  #include <string>
  #include "myheader.h"

  using std::vector;
  using std::string;
  ```
- **Not recommended for**: Never skip this rule - there are no valid exceptions
  ```cpp
  // NEVER do this - no exceptions!
  using namespace std;  // WRONG!
  #include <iostream>   // May cause conflicts

  // Even in .cpp files, include first
  using std::string;    // WRONG ORDER!
  #include "myheader.h"
  ```

---

### Rule 60: Avoid allocating and deallocating memory in different modules

**Summary**: Memory allocated in one module (DLL, shared library, or translation unit) should be deallocated in the same module.

**Reason**: Different modules may use different heaps, different versions of allocators, or different memory management strategies. Crossing module boundaries with memory operations can cause crashes, memory corruption, or leaks.

**Example (good)**:
```cpp
// Good: Module A manages its own memory
// module_a.h
class Buffer {
public:
    Buffer(size_t size);
    ~Buffer();
    char* data() { return data_; }
private:
    char* data_;
    size_t size_;
};

// module_a.cpp
Buffer::Buffer(size_t size) : size_(size) {
    data_ = new char[size];  // Allocated in Module A
}

Buffer::~Buffer() {
    delete[] data_;  // Deallocated in Module A
}

// Or use factory pattern with deleter
// module_a.h
std::unique_ptr<char[], void(*)(char*)> createBuffer(size_t size);

// module_a.cpp
std::unique_ptr<char[], void(*)(char*)> createBuffer(size_t size) {
    return std::unique_ptr<char[], void(*)(char*)>(
        new char[size],
        [](char* p) { delete[] p; }  // Deleter in same module
    );
}
```

**Example (main) - Using the good code above**:
```cpp
// main.cpp (links to module_a.dll)
#include "module_a.h"

int main() {
    // Good Example 1: Using RAII Buffer class
    {
        Buffer buf(1024);  // Constructor allocates in module_a
        char* data = buf.data();
        // ... use buffer ...
        // ✓ SAFE: Destructor automatically deallocates in module_a when buf goes out of scope
    }

    // Good Example 2: Using smart pointer with custom deleter
    {
        auto buf = createBuffer(1024);  // Returns unique_ptr with custom deleter
        char* data = buf.get();
        // ... use buffer ...
        // ✓ SAFE: Custom deleter (lambda) runs in module_a's context
        //         Memory allocated and deleted in same module
    }

    return 0;
}

// ✓ Why this works correctly:
//
// Solution 1: RAII ensures same-module allocation/deallocation
// - Buffer constructor calls 'new' in module_a's heap
// - Buffer destructor calls 'delete' in module_a's heap
// - No memory crosses module boundary (only the Buffer object exists in main)
//
// Solution 2: Custom deleter captures allocation context
// - unique_ptr stores the deleter function alongside the pointer
// - Deleter lambda is compiled into module_a.dll
// - When unique_ptr destructs, it calls module_a's deleter
// - Deleter runs in module_a's context, using module_a's heap
//
// Key principle: Memory lifetime managed by the module that created it
```

**Example (bad)**:
```cpp
// Bad Example 1: Returning raw pointer from module function
// module_a.h
char* createBuffer(size_t size);

// module_a.cpp (compiled as DLL/shared library)
char* createBuffer(size_t size) {
    return new char[size];  // Allocated using Module A's heap
}

// Bad Example 2: Passing ownership across module boundary
// module_a.h
class DataProcessor {
public:
    char* processData();  // Returns allocated memory
};

// module_a.cpp (compiled as DLL/shared library)
char* DataProcessor::processData() {
    char* result = new char[1024];  // Allocated in module_a's heap
    // ... process ...
    return result;  // Returns pointer to module_a's heap memory
}
```

**Example (main) - Using the bad code above**:
```cpp
// main.cpp (links to module_a.dll)
#include "module_a.h"

int main() {
    // Bad Example 1 usage:
    char* buf = createBuffer(1024);  // Memory allocated in module_a's heap
    // ... use buffer ...
    delete[] buf;  // ❌ ERROR: Deallocating in main's heap!

    // Bad Example 2 usage:
    DataProcessor processor;
    char* data = processor.processData();  // Got pointer from module_a
    // ... use data ...
    delete[] data;  // ❌ ERROR: Deleting module_a's memory from main's heap!

    return 0;
}

// ❌ Why this causes problems:
//
// Problem 1: Different Heap Allocators
// - module_a.dll uses its own C runtime library (CRT) with its own heap
// - main.exe uses a different CRT instance with a separate heap
// - Deleting memory from heap A using heap B's allocator causes:
//   * Heap corruption (internal heap structures get corrupted)
//   * Crashes (segmentation fault or access violation)
//   * Memory leaks (heap can't track/free memory properly)
//
// Problem 2: Compiler/Runtime Mismatches
// - module_a.dll compiled with Visual Studio 2019 (/MD)
// - main.exe compiled with Visual Studio 2022 (/MD)
// - Different versions = different heap implementations
// - Result: Undefined behavior, likely crash
//
// Problem 3: Debug/Release Mismatch
// - module_a.dll compiled in Release mode
// - main.exe compiled in Debug mode
// - Debug CRT uses special allocators with guards/tracking
// - Release CRT uses optimized allocators
// - Mixing them = guaranteed crash
//
// Real-world consequence on Windows:
// - Each DLL loaded with /MD gets its own heap from msvcr*.dll
// - new in module_a.dll calls heap_A's allocate()
// - delete in main.exe calls heap_B's deallocate()
// - heap_B crashes because pointer is not in its memory pool
```


**Decision**:
- **Good for**: DLLs/shared libraries with separate heaps, cross-module interfaces, plugin architectures
  ```cpp
  // Good: Plugin system with proper memory management
  // plugin_interface.h
  class IPlugin {
  public:
      virtual ~IPlugin() = default;
      virtual void execute() = 0;
  };

  // Factory functions ensure same module allocation/deallocation
  extern "C" IPlugin* createPlugin();
  extern "C" void destroyPlugin(IPlugin* p);

  // plugin.cpp
  extern "C" IPlugin* createPlugin() {
      return new MyPlugin();  // Allocated here
  }

  extern "C" void destroyPlugin(IPlugin* p) {
      delete p;  // Deallocated in same module
  }
  ```
- **Not recommended for**: Single executables with unified memory management, internal APIs within same binary
  ```cpp
  // Not necessary: Single executable with unified heap
  // utils.h (same executable, same heap)
  class Helper {
      std::vector<int> data;  // OK: all in same binary
  public:
      Helper();
      ~Helper();
  };

  // main.cpp
  void process() {
      Helper* h = new Helper();  // OK: same heap
      delete h;  // OK: single executable, same allocator
  }
  ```

---

### Rule 61: Don't define entities with linkage in a header file

**Summary**: Header files should only declare entities, not define them (except for inline functions, templates, and constexpr).

**Reason**: Headers are included in multiple translation units. Non-inline definitions create multiple definitions of the same symbol, violating ODR (One Definition Rule) and causing linker errors or undefined behavior.

**Example (good)**:
```cpp
// Good header.h:
extern int globalVar;              // Declaration only
void func();                       // Declaration only

inline void inlineFunc() {         // OK: inline
    // ...
}

template<typename T>               // OK: template
void templateFunc(T t) {
    // ...
}

constexpr int constantValue = 42;  // OK: constexpr (implicitly inline)

class MyClass {
    static const int value = 10;   // OK: inline static member
    void memberFunc();             // Declaration
};

// Corresponding .cpp file:
// source.cpp
int globalVar = 0;                 // Definition in .cpp
void func() { /* ... */ }          // Definition in .cpp
void MyClass::memberFunc() { }     // Definition in .cpp
```

**Example (bad)**:
```cpp
// Bad header.h:
int globalVar = 0;            // ERROR: Definition in header!
void func() { /* ... */ }     // ERROR: Non-inline definition!

static int staticVar = 0;     // BAD: Creates copy in each TU (ODR violation)

class MyClass {
    void memberFunc() { }     // BAD: Implicitly inline but not best practice
};

// ❌ Problems this causes:
// - Multiple translation units including this header will each get their own copy
// - Linker error: "multiple definition of 'globalVar'" or "multiple definition of 'func'"
// - staticVar wastes memory (separate copy in each TU)
// - Undefined behavior due to ODR violation
```

**Fix 1: Move definitions to .cpp file**:
```cpp
// header.h
extern int globalVar;      // Declaration only
void func();               // Declaration only

class MyClass {
    void memberFunc();     // Declaration only
};

// source.cpp
int globalVar = 0;         // Definition in .cpp
void func() { /* ... */ }  // Definition in .cpp
void MyClass::memberFunc() { }  // Definition in .cpp
```

**Fix 2: Use inline for functions (if small)**:
```cpp
// header.h
extern int globalVar;  // Still needs .cpp definition

inline void func() {   // inline = OK in header
    /* small implementation */
}

class MyClass {
    inline void memberFunc() { }  // Explicitly inline
};
```

**Fix 3: Use constexpr for compile-time constants**:
```cpp
// header.h
constexpr int staticVar = 0;  // constexpr = implicitly inline, OK in header
```

**Decision**:
- **Good for**: Declarations, inline functions, templates, constexpr values in headers
  ```cpp
  // header.h - All good practices
  extern int globalCounter;  // Declaration

  inline int increment() {   // Inline function OK
      return ++globalCounter;
  }

  template<typename T>       // Template OK
  T max(T a, T b) {
      return (a > b) ? a : b;
  }

  constexpr double PI = 3.14159;  // constexpr OK

  class Widget {
      static constexpr int MAX = 100;  // OK
      void process();  // Declaration only
  };
  ```
- **Not recommended for**: Non-inline function definitions, global variable definitions, static variables with external linkage
  ```cpp
  // bad_header.h - All violations
  int counter = 0;  // WRONG: global definition in header

  void process() {  // WRONG: non-inline function definition
      ++counter;
  }

  static int s_value = 42;  // WRONG: each TU gets own copy (wastes memory)

  const char* message = "Hello";  // WRONG: needs extern or constexpr

  class Widget {
      void doWork() { /*...*/ }  // WRONG: define in .cpp instead
  };
  ```

---

### Rule 62: Don't allow exceptions to propagate across module boundaries

**Summary**: Exceptions should be caught and handled before crossing module (DLL/shared library) boundaries. Use error codes or other mechanisms for inter-module error reporting.

**Reason**: Different modules may be compiled with different compilers, different exception handling mechanisms, or different runtime libraries. Exception propagation across these boundaries can cause crashes or undefined behavior.

**Why exceptions don't cross module boundaries safely**:

C++ exceptions rely on several compiler and runtime-specific mechanisms:

1. **Exception Handling ABI** (Application Binary Interface)
   - MSVC uses SEH (Structured Exception Handling) on Windows
   - GCC/Clang use Itanium C++ ABI with DWARF unwinding on Linux
   - These mechanisms are fundamentally incompatible
   - An exception thrown using SEH cannot be caught by DWARF-based handlers

2. **RTTI (Run-Time Type Information) and Type Matching**
   - Exception catch blocks use `typeid()` to match exception types
   - Each module has its own copy of type information for `std::exception`
   - Type matching compares addresses of type_info objects
   - Different modules = different addresses = match fails

3. **vtable Pointers and Virtual Functions**
   - Exception objects (like `std::exception`) have virtual functions
   - Each module has its own vtable for standard exception classes
   - Crossing boundaries means vtable pointers reference wrong module's memory
   - Calling `e.what()` may read garbage or crash

4. **Stack Unwinding Metadata**
   - Compilers generate unwind tables (.eh_frame, .pdata, .xdata sections)
   - These describe how to safely unwind the stack during exception propagation
   - Each module has its own unwind tables
   - Cross-module unwinding may not find correct cleanup code

5. **Memory Management for Exception Objects**
   - Exception objects are allocated on a special exception heap
   - Different runtimes use different exception heaps
   - Similar to Rule #60's memory issue across modules

**When this matters most**:
- Plugin systems (host app + dynamically loaded plugins)
- Shared libraries (.so, .dll) used by multiple applications
- Mixed compiler environments (MSVC + GCC components)
- Different C++ standard library versions (libstdc++ vs libc++ vs MSVC STL)

**Example (good)**:
```cpp
// Good: Catch exceptions at module boundary
// module_interface.h
enum ErrorCode { SUCCESS, INVALID_INPUT, PROCESSING_ERROR, OUT_OF_MEMORY };

ErrorCode processData(const char* data, size_t size) noexcept;
const char* getLastError() noexcept;

// module_impl.cpp
thread_local std::string lastError;

ErrorCode processData(const char* data, size_t size) noexcept {
    try {
        if (!data || size == 0) {
            lastError = "Invalid input";
            return INVALID_INPUT;
        }
        internalProcess(data, size);  // May throw internally
        return SUCCESS;
    } catch (const std::bad_alloc&) {
        lastError = "Out of memory";
        return OUT_OF_MEMORY;
    } catch (const std::exception& e) {
        lastError = e.what();
        return PROCESSING_ERROR;
    } catch (...) {
        lastError = "Unknown error";
        return PROCESSING_ERROR;
    }
}

const char* getLastError() noexcept {
    return lastError.c_str();
}
```

**Example (bad)**:
```cpp
// Bad: Exceptions propagate across module boundary
// module_interface.h
void processData(const char* data);  // No noexcept!

// module_impl.cpp (compiled as DLL)
void processData(const char* data) {
    if (!data) {
        throw std::invalid_argument("null data");  // BAD: Crosses boundary!
    }
    throw std::runtime_error("error");  // BAD: Propagates to caller
}

// main.cpp (different module/binary)
int main() {
    try {
        processData(myData);  // PROBLEM: May crash or fail to catch
    } catch (const std::exception& e) {
        // May not catch if exception models differ between modules!
        // Different compilers/runtime = different exception handling
    }
}

// ❌ Why this causes problems:
//
// Problem 1: Different Exception Handling Mechanisms
// - Module A compiled with MSVC (uses SEH - Structured Exception Handling)
// - Module B compiled with GCC (uses Itanium ABI with DWARF unwinding)
// - Throwing from A to B: exception objects are incompatible
// - Result: std::terminate() called, program crashes
//
// Problem 2: Different Runtime Libraries (CRT mismatch)
// - module_impl.dll uses /MD (dynamic CRT - msvcr140.dll)
// - main.exe uses /MT (static CRT - CRT linked into executable)
// - Exception thrown: std::invalid_argument created in msvcr140.dll's heap
// - Catch block: tries to match against main.exe's statically linked std::invalid_argument
// - Type info pointers differ (different memory addresses)
// - Result: catch fails, or crash during RTTI lookup
//
// Problem 3: vtable and RTTI Incompatibility
// - module_impl.dll has its own std::exception vtable at address 0x1000
// - main.exe has its own std::exception vtable at address 0x2000
// - Exception object created with vtable pointer = 0x1000
// - Catch handler checks: "Is this std::exception?" by comparing vtable
// - Comparison fails because vtables are at different addresses
// - Result: Exception not caught, std::unexpected() or std::terminate()
//
// Problem 4: ABI Incompatibility
// - DLL compiled with Visual Studio 2019 (v142 toolset)
// - EXE compiled with Visual Studio 2022 (v143 toolset)
// - Exception object layout changed between versions
// - what() function offset in vtable changed
// - Result: Accessing e.what() reads garbage memory, crash
//
// Problem 5: Stack Unwinding Issues
// - Exception thrown in module_impl.dll
// - Stack unwinding begins, traversing call stack
// - Unwind info (.pdata/.xdata sections) in DLL
// - Main.exe doesn't have correct unwind info for DLL's stack frames
// - Result: Unwinding corrupts stack, crash during cleanup
//
// Real-world scenario:
// Windows DLL compiled with /EHsc, main.exe with /EHa
// - /EHsc: only catches C++ exceptions
// - /EHa: catches both C++ and SEH exceptions
// - Exception model mismatch causes std::terminate()
```

**Alternative Approaches for Error Handling Across Modules**:

```cpp
// Approach 1: Error codes (C-style, most portable)
enum ErrorCode { SUCCESS = 0, ERR_INVALID_INPUT = -1, ERR_NOMEM = -2 };
ErrorCode module_function(const char* input) noexcept;

// Approach 2: Error codes + error message retrieval
ErrorCode module_function(const char* input) noexcept;
const char* module_getLastError() noexcept;  // Thread-safe with thread_local

// Approach 3: std::optional/std::expected (C++17/C++23)
// module_interface.h
#include <optional>
struct Result {
    int value;
    int errorCode;
};
Result module_function(const char* input) noexcept;  // Check errorCode == 0 for success

// Approach 4: Output parameter + boolean success
bool module_function(const char* input, int* output, char* errorBuffer, size_t bufferSize) noexcept;

// Approach 5: Callback for error reporting
typedef void (*ErrorCallback)(const char* message);
void module_setErrorCallback(ErrorCallback callback);
void module_function(const char* input) noexcept;  // Calls callback on error
```

**Decision**:
- **Good for**: DLL/shared library interfaces, plugin APIs, C-compatible interfaces
  ```cpp
  // Good: DLL with error codes
  // graphics_dll.h
  enum RenderError { RENDER_OK, RENDER_INVALID_PARAMS, RENDER_OUT_OF_MEMORY };

  extern "C" {
      RenderError renderScene(const char* sceneName) noexcept;
      const char* getError() noexcept;
  }

  // Implementation catches all exceptions at boundary
  RenderError renderScene(const char* sceneName) noexcept {
      try {
          if (!sceneName) return RENDER_INVALID_PARAMS;
          internalRender(sceneName);  // May throw internally
          return RENDER_OK;
      } catch (const std::bad_alloc&) {
          return RENDER_OUT_OF_MEMORY;
      } catch (...) {
          return RENDER_INVALID_PARAMS;
      }
  }
  ```
- **Not recommended for**: Internal APIs within same binary, header-only libraries
  ```cpp
  // Not necessary: Same binary (no module boundary)
  // internal_utils.h (header-only)
  namespace internal {
      template<typename T>
      void processData(const T& data) {
          if (data.empty()) {
              throw std::invalid_argument("empty");  // OK: same binary
          }
      }
  }

  // app.cpp (same binary)
  void run() {
      try {
          internal::processData(myData);  // OK: same exception model
      } catch (const std::exception& e) {
          // Safe: consistent exception handling
      }
  }
  ```

**Key Takeaway**: Use exceptions freely *within* modules, but translate them to error codes at module boundaries. Mark boundary functions with `noexcept` to enforce this at compile time.

---

### Rule 63: Use sufficiently portable types in a module's interface

**Summary**: Module interfaces should use types that are portable and well-defined across different platforms, compilers, and build configurations.

**Reason**: Module boundaries often represent ABI (Application Binary Interface) boundaries. Using non-portable types can cause issues when modules are compiled with different settings or on different platforms, leading to data corruption, crashes, or undefined behavior.

**Example (good)**:
```cpp
// Good: Portable types in interface
#include <cstdint>

// module_interface.h
extern "C" {
    // Use fixed-width types from <cstdint>
    int32_t module_process(const char* data, uint32_t size);

    // Use explicit-size types for counts and sizes
    uint64_t module_getCount(void);

    // Use int for boolean (0=false, non-zero=true)
    int32_t module_setEnabled(int32_t enabled);

    // Use opaque pointers for complex types
    typedef struct ModuleHandle* ModuleHandle;
    ModuleHandle module_create(void);
    void module_destroy(ModuleHandle handle);
}

// Or C++ with portable types
namespace module {
    class Interface {
    public:
        virtual int32_t process(const char* data, uint32_t size) = 0;
        virtual ~Interface() = default;
    };

    Interface* createInterface();  // Factory function
    void destroyInterface(Interface* iface);
}
```

**Example (bad)**:
```cpp
// Bad: Non-portable types in interface
// module_interface.h

void process(size_t count);        // BAD: size_t is 32-bit on some platforms, 64-bit on others
long getValue();                    // BAD: long is 32-bit on Windows, 64-bit on Linux
unsigned long long getId();         // BAD: size varies

bool setFlag(bool flag);           // BAD: bool size is implementation-defined
                                    // (usually 1 byte but not guaranteed)

enum Status { OK, ERROR };         // BAD: enum size is implementation-defined
                                    // (could be int, short, or other)

void* allocate(int size);          // BAD: pointer size varies (32-bit vs 64-bit)
                                    // (use opaque handles instead)

class Config {                     // BAD: class layout is compiler-specific
    std::string name;               // BAD: std::string has no stable ABI across compilers
    std::vector<int> values;        // BAD: std::vector layout varies
};
```

**Decision**:
- **Good for**: DLL/shared library interfaces, cross-platform modules, stable ABIs, C-compatible interfaces
  ```cpp
  // Good: Cross-platform DLL interface
  // platform_api.h
  #include <cstdint>

  extern "C" {
      // Portable across 32/64-bit platforms
      int32_t api_initialize(void);
      int32_t api_process(const char* data, uint32_t length);
      uint64_t api_getTimestamp(void);

      // Opaque handle (pointer size doesn't matter)
      typedef struct Context* ContextHandle;
      ContextHandle api_createContext(void);
      void api_destroyContext(ContextHandle ctx);
  }

  // Works identically on Windows (32/64), Linux, macOS, etc.
  ```
- **Not recommended for**: Internal APIs within same compilation unit, header-only libraries compiled together, modules always compiled with identical settings
  ```cpp
  // Not necessary: Internal header-only library
  // internal_algorithm.h (compiled together with main project)
  namespace algorithm {
      template<typename T>
      void sort(std::vector<T>& data) {
          // OK to use std::vector, size_t internally
          for (size_t i = 0; i < data.size(); ++i) {
              // ... sorting logic ...
          }
      }
  }

  // main.cpp (same compiler, same settings)
  void process() {
      std::vector<int> values = {3, 1, 4, 1, 5};
      algorithm::sort(values);  // OK: compiled together
  }
  ```

---

## Quick Reference Chart

| Rule | Topic | Key Takeaway |
|------|-------|-------------|
| 57 | Interface placement | Keep type and its interface in same namespace |
| 58 | Namespace organization | Separate unrelated functionality |
| 59 | Include order | `#include` before `using` |
| 60 | Module memory management | Allocate and deallocate in same module |
| 61 | Header file definitions | Don't define entities with linkage in headers |
| 62 | Module exception handling | Don't propagate exceptions across module boundaries |
| 63 | Module interface types | Use portable types in module interfaces |

---

## Common Themes

These rules share several important themes:

**Rules 57-59 (Namespaces):**
1. **Explicit is better than implicit**: Clear code is maintainable code
2. **Minimize namespace pollution**: Keep global namespace clean
3. **Organize by relationship**: Group related functionality, separate unrelated concerns

**Rules 60-63 (Modules):**
1. **Module boundaries are trust boundaries**: Different modules may have different implementations
2. **ABI stability matters**: Module interfaces must be stable across different compilation settings
3. **Defensive programming**: Modules should not make assumptions about each other's internals
4. **Cross-platform compatibility**: Module interfaces should work across different platforms and compilers

---

## Resources

- **Book**: "C++ Coding Standards: 101 Rules, Guidelines, and Best Practices" by Herb Sutter and Andrei Alexandrescu (2005)
- **Related Topics**: ADL (Argument-Dependent Lookup), ODR (One Definition Rule), Translation units, Linkage

---

## License

These examples are for educational purposes, demonstrating the namespace rules from the C++ Coding Standards book.
