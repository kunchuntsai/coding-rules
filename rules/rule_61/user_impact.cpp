// Demonstrates the impact on users when header has namespace-level using

// Scenario 1: User includes the bad header
// #include "bad_example.h"  // This forces "using namespace std" on this file!

// Now the user's code is affected:

namespace usercode {
    // These would cause conflicts if bad_example.h had "using namespace std"

    // User's own list class
    class list {  // Would conflict with std::list
    public:
        void add(int x) { }
    };

    // User's own distance function
    int distance(int a, int b) {  // Would conflict with std::distance
        return (a > b) ? (a - b) : (b - a);
    }

    // User's own count function
    template<typename Container>
    int count(const Container& c) {  // Would conflict with std::count
        return c.size();
    }
}

// Scenario 2: User has their own using directives
// #include "bad_example.h"  // Forces std namespace

namespace graphics {
    class Color { };
    class Point { };
}

// User wants to use graphics namespace
// using namespace graphics;  // Now conflicts with std names from bad_example.h!

// Scenario 3: Multiple library headers with using
/*
#include "bad_example.h"     // Has "using namespace std"
#include "third_party_lib.h" // Also has "using namespace something_else"

// Now there's a mess of overlapping namespaces
// and potential conflicts between libraries!
*/

int main() {
    // With good_example.h:
    // Code is clear and explicit

    // With bad_example.h:
    // Namespace pollution makes it unclear what's what
    // Potential conflicts and ambiguities

    return 0;
}

/*
Key Takeaway:

Headers are interfaces. They should:
- NOT impose namespace decisions on users
- Use fully qualified names
- Let users decide what to bring into scope
- Be self-documenting about where types come from

Implementation files (.cpp) have more freedom:
- Can use "using" at file scope (still discouraged)
- Can use "using" in function scope (preferred)
- Only affect that single translation unit
*/
