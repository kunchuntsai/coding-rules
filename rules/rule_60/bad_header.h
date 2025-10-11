// VERY BAD: using directive or declaration in header file

#ifndef BAD_HEADER_H
#define BAD_HEADER_H

#include <string>
#include <vector>

// NEVER DO THIS IN A HEADER!
using namespace std;  // Forces everyone who includes this header to use std namespace

// Even this is bad in a header:
// using std::string;  // Still pollutes namespace for all includers

class BadClass {
public:
    // Now anyone including this header has "using namespace std" active
    // This affects their entire translation unit!
    vector<string> getData() const;  // Should be std::vector<std::string>

private:
    vector<string> data_;  // Should be std::vector<std::string>
};

/*
Problems:
1. Forces namespace decisions on all users of this header
2. Can cause name conflicts in user code
3. Violates principle of least surprise
4. Makes it unclear what namespace things come from
5. Can cause different behavior in different translation units
*/

#endif // BAD_HEADER_H
