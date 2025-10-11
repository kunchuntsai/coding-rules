// Bad: Improper use of using-directives and declarations

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <set>

// BAD: using-directive brings in EVERYTHING from std
using namespace std;

// Now we have hundreds of names polluting our namespace:
// string, vector, list, map, set, cout, cin, endl,
// sort, find, count, distance, advance, begin, end,
// plus many more...

namespace myapp {
    // Potential conflicts with std names

    // What if we want our own "count" function?
    template<typename Container>
    int count(const Container& c) {
        return c.size();  // Different from std::count!
    }

    // What if we want our own "distance" function?
    int distance(int a, int b) {
        return abs(a - b);  // Different from std::distance!
    }

    // These can cause ambiguity errors
    void problematicFunction() {
        vector<int> numbers = {1, 2, 3, 2, 4, 2};

        // Which count? std::count or myapp::count?
        // Depends on ADL and overload resolution - confusing!
        // auto c = count(numbers);  // AMBIGUOUS!

        // Have to explicitly qualify
        auto c1 = myapp::count(numbers);
        auto c2 = std::count(numbers.begin(), numbers.end(), 2);
    }
}

// BAD: Multiple using-directives can conflict
namespace graphics {
    class Point {
    public:
        double x, y;
    };
}

namespace geometry {
    class Point {
    public:
        double x, y, z;
    };
}

namespace bad_code {
    using namespace graphics;
    using namespace geometry;

    void ambiguousFunction() {
        // Which Point? graphics::Point or geometry::Point?
        // Point p;  // ERROR: ambiguous!

        // Have to explicitly qualify anyway
        graphics::Point p1;
        geometry::Point p2;
    }
}

// BAD: using-directive in narrow scope is confusing
namespace confusing {
    void processData() {
        {
            using namespace std;
            // Now std is available...
            vector<string> data = {"one", "two"};
            cout << data.size() << "\n";
        }

        // But not here - this is confusing for readers
        // vector<int> numbers;  // ERROR: vector not found

        // Readers have to track where the using-directive is active
    }
}

int main() {
    // With "using namespace std", it's unclear what comes from where
    vector<string> items = {"a", "b", "c"};
    sort(items.begin(), items.end());

    for (const auto& item : items) {
        cout << item << endl;
    }

    // Better to be explicit or use specific using-declarations

    return 0;
}
