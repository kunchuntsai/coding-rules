// Bad: Using old C-style static for internal linkage

#include <iostream>
#include <string>
#include <vector>

// Old C-style: using static for internal linkage

// Can't use static with class definitions!
// static class Logger { ... };  // ERROR: doesn't work!

// Workaround: have to define class normally, but it's not truly internal
class Logger {  // PROBLEM: External linkage by default
public:
    void log(const std::string& message) {
        std::cout << "[LOG] " << message << "\n";
    }
};

// Static function (C-style)
static bool isValidInput(const std::string& input) {
    return !input.empty() && input.length() < 100;
}

// Static constant (C-style)
static constexpr int MAX_RETRIES = 3;

// Can't use static with struct definitions either!
// static struct CacheEntry { ... };  // ERROR: doesn't work!

struct CacheEntry {  // PROBLEM: External linkage by default
    std::string key;
    std::string value;
    long timestamp;
};

// Static variables (C-style)
static Logger logger;
static std::vector<CacheEntry> cache;

// Static helper function
static void logError(const std::string& error) {
    logger.log("ERROR: " + error);
}

// Public interface functions
namespace mylib {

void processData(const std::string& data) {
    if (!isValidInput(data)) {
        logError("Invalid input");
        return;
    }

    logger.log("Processing: " + data);

    for (int i = 0; i < MAX_RETRIES; ++i) {
        logger.log("Attempt " + std::to_string(i + 1));
    }
}

void cacheData(const std::string& key, const std::string& value) {
    CacheEntry entry{key, value, 0};
    cache.push_back(entry);
    logger.log("Cached: " + key);
}

} // namespace mylib

int main() {
    mylib::processData("test data");
    mylib::cacheData("key1", "value1");
    return 0;
}

/*
Problems with static approach:

1. Inconsistent:
   - Works for functions ✓
   - Works for variables ✓
   - Does NOT work for classes ✗
   - Does NOT work for structs ✗
   - Does NOT work for type aliases ✗

2. Mixed idioms:
   - Some things are static
   - Some things can't be static
   - Confusing and inconsistent

3. C-style in C++ code:
   - Not the modern C++ way
   - Deprecated by unnamed namespaces

4. Less clear intent:
   - Is it internal linkage or internal state?
   - Unnamed namespace is clearer

5. Can't group:
   - Each item needs its own static keyword
   - Unnamed namespace groups related internals
*/
