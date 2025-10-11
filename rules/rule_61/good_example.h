// Good: No namespace-level using in header

#ifndef GOOD_EXAMPLE_H
#define GOOD_EXAMPLE_H

#include <string>
#include <vector>
#include <memory>
#include <iosfwd>

// NO using directives or declarations at namespace scope!

namespace mylib {

// Option 1: Use fully qualified names (preferred)
class DataStore {
public:
    void addItem(const std::string& item);
    std::vector<std::string> getItems() const;
    std::shared_ptr<std::string> getItem(std::size_t index) const;

    void print(std::ostream& os) const;

private:
    std::vector<std::string> items_;
};

// Option 2: Type aliases for complex types (acceptable)
using DataStorePtr = std::shared_ptr<DataStore>;
using StringList = std::vector<std::string>;

class DataManager {
public:
    void addStore(DataStorePtr store);
    StringList getAllItems() const;

private:
    std::vector<DataStorePtr> stores_;
};

// Option 3: using in function scope (OK in inline functions)
inline void DataStore::print(std::ostream& os) const {
    using std::endl;  // OK: limited to this function scope

    for (const auto& item : items_) {
        os << item << endl;
    }
}

} // namespace mylib

// Operator in same namespace as type (Rule 57)
namespace mylib {
    std::ostream& operator<<(std::ostream& os, const DataStore& store);
}

#endif // GOOD_EXAMPLE_H
