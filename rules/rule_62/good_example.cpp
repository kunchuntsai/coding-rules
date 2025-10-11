// Good: Appropriate use of using-declarations (or avoiding them)

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

// Best practice: Fully qualified names (most explicit)
namespace example1 {
    void processStrings() {
        std::vector<std::string> names = {"Charlie", "Alice", "Bob"};
        std::sort(names.begin(), names.end());

        for (const auto& name : names) {
            std::cout << name << std::endl;
        }
    }
}

// Good: using-declarations in implementation file at file scope
// (acceptable for frequently used types in a .cpp file)
namespace example2 {
    using std::string;
    using std::vector;
    using std::cout;

    void processStrings() {
        vector<string> names = {"Charlie", "Alice", "Bob"};
        std::sort(names.begin(), names.end());

        for (const auto& name : names) {
            cout << name << std::endl;
        }
    }
}

// Best: using-declarations in function scope
namespace example3 {
    void processStrings() {
        using std::vector;
        using std::string;

        vector<string> names = {"Charlie", "Alice", "Bob"};
        std::sort(names.begin(), names.end());

        // Bring cout into scope only where needed
        {
            using std::cout;
            using std::endl;

            for (const auto& name : names) {
                cout << name << endl;
            }
        }
    }

    // Another function doesn't get polluted with previous using declarations
    void processNumbers() {
        using std::vector;
        using std::cout;

        vector<int> numbers = {5, 2, 8, 1, 9};
        std::sort(numbers.begin(), numbers.end());

        for (int n : numbers) {
            cout << n << " ";
        }
        cout << "\n";
    }
}

// Good: using-declarations for specific, frequently used types
namespace example4 {
    using StringPtr = std::shared_ptr<std::string>;
    using StringVector = std::vector<std::string>;

    class TextProcessor {
    public:
        void addText(StringPtr text) {
            texts_.push_back(text);
        }

        StringVector getAllTexts() const {
            StringVector result;
            for (const auto& ptr : texts_) {
                if (ptr) {
                    result.push_back(*ptr);
                }
            }
            return result;
        }

    private:
        std::vector<StringPtr> texts_;
    };
}

int main() {
    example1::processStrings();
    example2::processStrings();
    example3::processStrings();
    example3::processNumbers();

    return 0;
}
