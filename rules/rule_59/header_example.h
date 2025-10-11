// Example showing why this matters in header files

#ifndef HEADER_EXAMPLE_H
#define HEADER_EXAMPLE_H

// NEVER do this in a header file!
// using namespace std;  // WRONG!

#include <string>
#include <vector>

// CORRECT: Use fully qualified names in header files
class DataProcessor {
public:
    void process(const std::string& data);
    std::vector<std::string> getResults() const;

private:
    std::vector<std::string> results_;
};

// If you must use using declarations in headers (discouraged),
// put them AFTER includes and preferably in a namespace or function scope

namespace impl {
    // More acceptable: using in a namespace scope (but still be careful)
    using StringList = std::vector<std::string>;
}

#endif // HEADER_EXAMPLE_H
