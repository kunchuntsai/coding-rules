// Bad: using directives BEFORE includes
// This file demonstrates concrete problems that occur when using comes before includes

// ============================================================================
// EXAMPLE 1: Basic violation - using before includes
// ============================================================================

// WRONG: using before includes!
using namespace std;

// These headers are now affected by the using directive
// This can cause unexpected behavior and name conflicts
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/*
Problems this causes:
1. The headers might define functions that conflict with std names
2. Future library changes might introduce conflicts
3. Makes it hard to understand what namespace things come from
4. Can cause different behavior depending on what's in the headers
*/

void example1() {
    vector<string> names = {"Alice", "Bob", "Charlie"};

    for (const auto& name : names) {
        cout << name << "\n";
    }
}

// ============================================================================
// EXAMPLE 2: Concrete conflict scenario - custom header with std-like names
// ============================================================================

// Simulate what happens with a custom header that might have std-like names
namespace my_lib {
    // Custom string class that might be in your header
    class string {
    public:
        string(const char*) {}
        const char* c_str() const { return "custom_string"; }
    };

    // Custom distance function
    template<typename T>
    int distance(T a, T b) { return 42; }
}

// BAD: using before including hypothetical headers
using namespace std;
using namespace my_lib;  // Now we have ambiguity!

// If these were actual #includes, the above using directives would affect them
// #include "my_lib/string.h"  // Contains my_lib::string
// #include <string>            // Contains std::string

void example2() {
    // AMBIGUITY: Which string? std::string or my_lib::string?
    // This won't compile due to ambiguous symbol
    // string text = "Hello";  // ERROR: ambiguous!

    // Have to use fully qualified names anyway, defeating the purpose
    std::string std_text = "Hello";
    my_lib::string custom_text = "World";
}

// ============================================================================
// EXAMPLE 3: Hidden dependencies and unclear code
// ============================================================================

// WRONG: Multiple using directives before includes
using namespace std;
using std::chrono::duration;

// Now it's unclear which names come from which headers
#include <chrono>
#include <thread>
#include <iostream>

void example3() {
    // Where does 'duration' come from? Hard to tell because using came first
    // Where does 'cout' come from? Already in scope before the include!
    auto d = duration<int, std::milli>(100);
    cout << "Duration: " << d.count() << "ms\n";
}

// ============================================================================
// EXAMPLE 4: Order-dependent behavior
// ============================================================================

// The behavior can change based on header inclusion order when using comes first

// WRONG: using declaration before includes
using std::swap;

// If a header defines its own swap, it might interfere
#include <algorithm>
#include <utility>

namespace custom {
    struct Point { int x, y; };

    // Custom swap for Point
    void swap(Point& a, Point& b) {
        Point temp = a;
        a = b;
        b = temp;
    }
}

void example4() {
    using namespace custom;

    Point p1{1, 2}, p2{3, 4};

    // Which swap gets called? The one from using std::swap or custom::swap?
    // Behavior depends on when using std::swap was declared relative to includes
    swap(p1, p2);  // Ambiguous and confusing!
}

// ============================================================================
// EXAMPLE 5: Breaking code with library updates
// ============================================================================

/*
Scenario: You write code that works fine, then update your standard library

// Your original code (C++11):
using namespace std;
#include <vector>
#include <memory>

void process() {
    auto ptr = make_unique<int>(42);  // Might work
}

// After C++14 update, std::make_unique is added
// If using came before include, you might get different behavior
// or conflicts with custom make_unique implementations
*/

void example5() {
    // This illustrates why using before includes is fragile:
    // Library evolution can break your code in subtle ways
}

// ============================================================================
// EXAMPLE 6: Multiple using declarations create order sensitivity
// ============================================================================

// WRONG: using declarations scattered before includes
using std::cout;

#include <iostream>

using std::vector;

#include <vector>

using std::string;

#include <string>

void example6() {
    // This works, but it's confusing and error-prone
    // What happens if you reorder the includes?
    // What if an include depends on names from another?
    vector<string> data = {"test"};
    cout << data[0] << "\n";
}

// ============================================================================

int main() {
    cout << "=== Bad Examples of using before includes ===\n\n";

    example1();
    // example2();  // Would cause compilation error due to ambiguity
    example3();
    example4();
    example5();
    example6();

    cout << "\nThese examples compile but demonstrate why the practice is bad.\n";
    cout << "See good_example.cpp for the correct approach.\n";

    return 0;
}
