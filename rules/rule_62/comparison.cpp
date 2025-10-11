// Side-by-side comparison of using approaches

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ============================================================================
// Approach 1: using-directive (WORST)
// ============================================================================
namespace approach1_bad {
    using namespace std;  // Imports 100+ names

    void processData() {
        vector<string> data = {"one", "two", "three"};
        sort(data.begin(), data.end());
        for (const auto& s : data) cout << s << endl;
    }

    // Problems:
    // - Unclear where types come from
    // - Namespace pollution
    // - Risk of conflicts
    // - Future std additions might break code
}

// ============================================================================
// Approach 2: Multiple specific using-declarations (BETTER)
// ============================================================================
namespace approach2_okay {
    using std::vector;
    using std::string;
    using std::cout;
    using std::endl;

    void processData() {
        vector<string> data = {"one", "two", "three"};
        std::sort(data.begin(), data.end());  // std::sort not imported
        for (const auto& s : data) cout << s << endl;
    }

    // Better:
    // - Only imports what's needed
    // - Clear what names are imported
    // - Less pollution
    // - But still affects entire file scope
}

// ============================================================================
// Approach 3: Function-scope using-declarations (BEST)
// ============================================================================
namespace approach3_good {
    void processData() {
        using std::vector;
        using std::string;

        vector<string> data = {"one", "two", "three"};
        std::sort(data.begin(), data.end());

        {
            using std::cout;
            using std::endl;
            for (const auto& s : data) cout << s << endl;
        }
    }

    // Best:
    // - Limited scope
    // - Clear intent
    // - Minimal pollution
    // - Other functions unaffected
}

// ============================================================================
// Approach 4: Fully qualified (MOST EXPLICIT)
// ============================================================================
namespace approach4_best {
    void processData() {
        std::vector<std::string> data = {"one", "two", "three"};
        std::sort(data.begin(), data.end());

        for (const auto& s : data) {
            std::cout << s << std::endl;
        }
    }

    // Most explicit:
    // - Clear where everything comes from
    // - No namespace pollution at all
    // - Self-documenting
    // - Slightly more verbose
}

// ============================================================================
// Approach 5: Type aliases for complex types (RECOMMENDED FOR COMPLEX TYPES)
// ============================================================================
namespace approach5_pragmatic {
    // Use type aliases for frequently used complex types
    using StringVector = std::vector<std::string>;
    using StringPtr = std::shared_ptr<std::string>;

    void processData() {
        StringVector data = {"one", "two", "three"};
        std::sort(data.begin(), data.end());

        for (const auto& s : data) {
            std::cout << s << std::endl;
        }
    }

    // Pragmatic:
    // - Reduces verbosity for complex types
    // - Clearly named aliases
    // - Still explicit
    // - Easy to change underlying type
}

int main() {
    approach1_bad::processData();
    approach2_okay::processData();
    approach3_good::processData();
    approach4_best::processData();
    approach5_pragmatic::processData();

    return 0;
}

/*
Recommendation hierarchy (best to worst):

1. Fully qualified names (std::vector<std::string>)
   - Use for simple types in headers and short functions

2. Type aliases (using StringVector = std::vector<std::string>)
   - Use for complex or frequently-used types
   - Good documentation value

3. Function-scope using declarations
   - Use in implementation files for local convenience
   - Keeps pollution minimal

4. File-scope using declarations in .cpp files
   - Acceptable but less preferred
   - Only in implementation files, never headers

5. NEVER: using-directives (using namespace X)
   - Don't use these in production code
   - Maybe OK in main() for small examples/prototypes
*/
