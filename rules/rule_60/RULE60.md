  Rule #60: Avoid allocating and deallocating memory in different modules

  What it is: A principle about where memory allocation and deallocation should happen
  - Problem: Different modules (DLLs/shared libraries) may have separate heaps
  - Rule: Memory allocated in Module A must be deallocated in Module A

  RAII (Resource Acquisition Is Initialization)

  What it is: A C++ programming technique for managing resource lifetime
  - Principle: Tie resource lifetime to object lifetime
  - Mechanism: Acquire resources in constructor, release in destructor
  - Resources include: memory, file handles, locks, sockets, etc.

  The Relationship

  RAII is one technique to satisfy Rule #60:

  // Rule #60: The problem
  // module_a.h
  char* createBuffer(size_t size);  // ❌ Raw pointer crosses boundary

  // main.cpp
  char* buf = createBuffer(1024);  // Allocated in module_a's heap
  delete[] buf;  // ❌ Deallocated in main's heap - VIOLATES Rule #60!

  // ----------------------------------------

  // Using RAII to solve Rule #60
  // module_a.h
  class Buffer {  // ✓ RAII class
  public:
      Buffer(size_t size);   // Constructor allocates
      ~Buffer();             // Destructor deallocates
      char* data();
  private:
      char* data_;
  };

  // main.cpp
  {
      Buffer buf(1024);  // ✓ Allocation happens in module_a
      // use buf...
  }  // ✓ Deallocation happens in module_a (destructor called)
     // ✓ SATISFIES Rule #60: same module allocates and deallocates

  Other Solutions to Rule #60 (Not RAII)

  Rule #60 can also be satisfied without RAII:

  // Solution 2: Paired create/destroy functions (not RAII)
  // module_a.h
  char* module_createBuffer(size_t size);
  void module_destroyBuffer(char* buf);  // ✓ Explicit destroy function

  // main.cpp
  char* buf = module_createBuffer(1024);  // Allocated in module_a
  // use buf...
  module_destroyBuffer(buf);  // ✓ Deallocated in module_a
  // ✓ SATISFIES Rule #60 but NOT using RAII

  // ----------------------------------------

  // Solution 3: Return by value (not RAII)
  // module_a.h
  std::vector<char> createBuffer(size_t size);  // ✓ Copy/move by value

  // main.cpp
  auto buf = createBuffer(1024);  // Vector copied/moved to main's heap
  // ✓ SATISFIES Rule #60: no cross-boundary memory management
  // (but this isn't really RAII either - it's just value semantics)

  Summary

  | Concept  | Scope             | Purpose                                                    |
  |----------|-------------------|------------------------------------------------------------|
  | Rule #60 | Module boundaries | Prevent cross-module memory management issues              |
  | RAII     | Object lifetime   | Automatic resource management via constructors/destructors |

  RAII helps implement Rule #60, but they're different concepts:
  - Rule #60 = what (don't mix heaps across modules)
  - RAII = how (one way to achieve it)