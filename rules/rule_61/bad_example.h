// Bad: using at namespace level in header

#ifndef BAD_EXAMPLE_H
#define BAD_EXAMPLE_H

#include <string>
#include <vector>
#include <memory>
#include <iosfwd>

// TERRIBLE: using directive at global scope in header!
using namespace std;

// ALSO BAD: using declarations at global scope in header
// using std::string;
// using std::vector;

namespace mylib {

// Now all users of this header are forced to have these names polluted
class DataStore {
public:
    void addItem(const string& item);  // Relies on global using
    vector<string> getItems() const;
    shared_ptr<string> getItem(size_t index) const;

    void print(ostream& os) const;

private:
    vector<string> items_;
};

class DataManager {
public:
    void addStore(shared_ptr<DataStore> store);
    vector<string> getAllItems() const;

private:
    vector<shared_ptr<DataStore>> stores_;
};

} // namespace mylib

/*
Problems this causes:

1. Anyone who #includes this header gets "using namespace std"
2. Can cause name conflicts in user code:
   - User has own string class? Conflict!
   - User has own vector? Conflict!
   - User's code with "using namespace boost" might conflict

3. Example of user pain:

   // user_code.cpp
   #include "bad_example.h"  // Oops! Now has "using namespace std"

   namespace user {
       class list { };  // ERROR: conflicts with std::list
       void swap(int&, int&) { }  // ERROR: conflicts with std::swap
   }

4. Makes code harder to understand - where do types come from?

5. Can cause different behavior in different translation units

6. Violates principle of least surprise

NEVER DO THIS!
*/

#endif // BAD_EXAMPLE_H
