// Good: using directives AFTER includes

// Include directives come first
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Using declarations AFTER all includes
using std::string;
using std::vector;
using std::cout;

// Or using directive after includes (though less preferred than specific using declarations)
// using namespace std;

int main() {
    // Now we can use the names we've brought into scope
    vector<string> names = {"Alice", "Bob", "Charlie"};

    for (const auto& name : names) {
        cout << name << "\n";
    }

    return 0;
}
