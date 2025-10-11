// Good: Using unnamed namespace for internal linkage

#include <iostream>
#include <string>
#include <vector>

// Unnamed namespace - everything inside has internal linkage
namespace {

// Helper class only used in this file
class Logger {
public:
    void log(const std::string& message) {
        std::cout << "[LOG] " << message << "\n";
    }
};

// Helper function only used in this file
bool isValidInput(const std::string& input) {
    return !input.empty() && input.length() < 100;
}

// Constant only used in this file
constexpr int MAX_RETRIES = 3;

// Helper data structure
struct CacheEntry {
    std::string key;
    std::string value;
    long timestamp;
};

// File-local variable
Logger logger;
std::vector<CacheEntry> cache;

// Helper function using the file-local logger
void logError(const std::string& error) {
    logger.log("ERROR: " + error);
}

} // unnamed namespace

// Public interface functions
namespace mylib {

void processData(const std::string& data) {
    if (!isValidInput(data)) {  // Using unnamed namespace function
        logError("Invalid input");  // Using unnamed namespace function
        return;
    }

    logger.log("Processing: " + data);  // Using unnamed namespace variable

    // Use MAX_RETRIES from unnamed namespace
    for (int i = 0; i < MAX_RETRIES; ++i) {
        // Process data...
        logger.log("Attempt " + std::to_string(i + 1));
    }
}

void cacheData(const std::string& key, const std::string& value) {
    // Using CacheEntry from unnamed namespace
    CacheEntry entry{key, value, 0};
    cache.push_back(entry);
    logger.log("Cached: " + key);
}

} // namespace mylib

int main() {
    mylib::processData("test data");
    mylib::cacheData("key1", "value1");

    // Logger, isValidInput, etc. are not accessible here
    // They have internal linkage - only visible in this file

    return 0;
}

/*
Benefits of unnamed namespace:

1. Works with everything:
   - Functions ✓
   - Classes ✓
   - Variables ✓
   - Constants ✓
   - Type aliases ✓

2. Clear intent: "This is internal to this file"

3. No linker conflicts with same names in other files

4. Better encapsulation than static
*/
