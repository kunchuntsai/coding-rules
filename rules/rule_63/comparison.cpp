// Comparison: static vs unnamed namespace

// FILE: module_a.cpp
// ============================================================================

#include <iostream>
#include <string>

// Using unnamed namespace (PREFERRED)
namespace {
    // Everything here has internal linkage
    // Not visible outside this translation unit

    class InternalHelper {
    public:
        void doWork() { std::cout << "Module A helper\n"; }
    };

    void internalFunction() {
        std::cout << "Module A internal function\n";
    }

    constexpr int INTERNAL_CONSTANT = 42;

    InternalHelper helper;
}

// Public API
namespace module_a {
    void publicFunction() {
        internalFunction();  // OK: same translation unit
        helper.doWork();     // OK: same translation unit
        std::cout << "Constant: " << INTERNAL_CONSTANT << "\n";
    }
}

// ============================================================================
// FILE: module_b.cpp (hypothetical)
// ============================================================================

/*
// This is a separate translation unit
// It can have the same internal names without conflict!

namespace {
    // Same names as module_a, but different translation unit
    class InternalHelper {
    public:
        void doWork() { std::cout << "Module B helper\n"; }
    };

    void internalFunction() {
        std::cout << "Module B internal function\n";
    }

    constexpr int INTERNAL_CONSTANT = 99;

    InternalHelper helper;
}

namespace module_b {
    void publicFunction() {
        internalFunction();  // Calls THIS file's internalFunction
        helper.doWork();     // Uses THIS file's helper
        std::cout << "Constant: " << INTERNAL_CONSTANT << "\n";
    }
}

// No linker errors! Each file's unnamed namespace content
// is completely separate, even with identical names.
*/

// ============================================================================
// Comparison with static approach
// ============================================================================

namespace static_approach {

// With static keyword:

// static class Helper { ... };  // ERROR: Can't do this!

// Have to do this instead:
class Helper {  // WARNING: External linkage! Might conflict!
public:
    void doWork() { }
};

static void internalFunc() { }  // OK with static

static constexpr int CONST = 42;  // OK with static

static Helper helper;  // OK with static, but Helper class still has external linkage

/*
Problems:
1. Class Helper has external linkage (ODR violation risk)
2. If module_b.cpp also defines class Helper, linker error!
3. Inconsistent - some things static, some not
4. Mixing C-style (static) with C++ (classes)
*/

} // namespace static_approach

// ============================================================================
// Advanced: Mixing unnamed namespace with other features
// ============================================================================

namespace advanced {

// Can nest namespaces
namespace {
    namespace detail {
        void helperFunction() { }
    }
}

// Can use with inline functions
namespace {
    inline int square(int x) { return x * x; }
}

// Can declare templates
namespace {
    template<typename T>
    T max(T a, T b) {
        return (a > b) ? a : b;
    }
}

// Can use type aliases
namespace {
    using String = std::string;
    using StringVector = std::vector<std::string>;
}

// All have internal linkage!

} // namespace advanced

int main() {
    module_a::publicFunction();

    // These would be errors (internal to module_a.cpp):
    // internalFunction();  // ERROR: not visible
    // helper.doWork();     // ERROR: not visible
    // auto x = INTERNAL_CONSTANT;  // ERROR: not visible

    return 0;
}

/*
Summary:

Unnamed Namespace (Preferred):
+ Works for all declarations
+ Consistent C++ idiom
+ Clear intent
+ Groups related internals
+ Prevents ODR violations
- Slightly more verbose

Static (Deprecated for this use):
+ Familiar to C programmers
+ Slightly less verbose
- Doesn't work for types
- Inconsistent application
- C-style in C++ code
- Doesn't prevent ODR violations for types

Recommendation: Always use unnamed namespace in modern C++!
*/
