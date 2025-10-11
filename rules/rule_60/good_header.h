// Good: Fully qualified names in header file

#ifndef GOOD_HEADER_H
#define GOOD_HEADER_H

#include <string>
#include <vector>

// No using directives or declarations at namespace scope in headers!

class GoodClass {
public:
    // Fully qualified names make it clear where types come from
    std::vector<std::string> getData() const;

    // If you absolutely need a using declaration, put it in limited scope
    void process() const {
        using std::cout;  // OK: limited to this function
        cout << "Processing...\n";
    }

private:
    std::vector<std::string> data_;
};

// Type aliases are OK (and preferred for complex types)
namespace detail {
    using StringVector = std::vector<std::string>;
    // Now users can choose to use detail::StringVector or not
}

/*
Benefits:
1. Clear where all types come from
2. No namespace pollution
3. Users have full control over their own namespace
4. Predictable behavior across translation units
5. Self-documenting code
*/

#endif // GOOD_HEADER_H
