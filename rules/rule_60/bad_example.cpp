// Bad: Allocating and deallocating memory in different modules
// This demonstrates the WRONG way to handle memory across module boundaries

#include <iostream>
#include <string>

// ============================================================================
// EXAMPLE 1: Classic mistake - module A allocates, module B deallocates
// ============================================================================

// Simulating Module A's interface
namespace module_a {
    // BAD: Returns memory allocated in Module A
    char* createBuffer(size_t size) {
        return new char[size];  // Allocated on Module A's heap
    }

    std::string* createString(const char* str) {
        return new std::string(str);  // Allocated on Module A's heap
    }
}

// Simulating Module B's usage
namespace module_b {
    void processAndCleanup(char* buffer) {
        // Do some processing...
        std::cout << "Processing buffer...\n";

        // BAD: Deallocating Module A's memory in Module B!
        delete[] buffer;  // CRASH RISK! Different heap!
    }

    void useString(std::string* str) {
        std::cout << "Using string: " << *str << "\n";

        // BAD: Deallocating Module A's memory in Module B!
        delete str;  // CRASH RISK! Different allocator!
    }
}

// ============================================================================
// EXAMPLE 2: Container with cross-module memory issues
// ============================================================================

namespace module_a {
    class DataContainer {
    public:
        int* data;
        size_t size;

        // BAD: Returns container but data is allocated in Module A
        static DataContainer* create(size_t n) {
            DataContainer* container = new DataContainer();
            container->data = new int[n];  // Module A's heap
            container->size = n;
            return container;
        }
    };
}

namespace module_b {
    void processContainer(module_a::DataContainer* container) {
        // Use the data...
        for (size_t i = 0; i < container->size; ++i) {
            container->data[i] = static_cast<int>(i);
        }

        // BAD: Cleanup in different module!
        delete[] container->data;  // CRASH RISK!
        delete container;           // CRASH RISK!
    }
}

// ============================================================================
// EXAMPLE 3: std::string across module boundary (dangerous!)
// ============================================================================

// Module A allocates std::string
namespace module_a {
    // BAD: Different modules may have different std::string implementations
    std::string* allocateString() {
        return new std::string("Hello from Module A");
    }
}

// Module B deallocates
namespace module_b {
    void deleteString(std::string* str) {
        // BAD: std::string's internal buffer might be on Module A's heap
        // Even though we're in Module B, the destructor might crash
        delete str;  // CRASH RISK!
    }
}

// ============================================================================
// EXAMPLE 4: Smart pointer confusion
// ============================================================================

#include <memory>

namespace module_a {
    // BAD: Returns shared_ptr but deleter uses Module A's heap
    std::shared_ptr<int> createSharedInt() {
        return std::make_shared<int>(42);  // Uses Module A's allocator
    }
}

namespace module_b {
    void useSharedInt() {
        auto ptr = module_a::createSharedInt();

        // When ptr goes out of scope, the deleter runs in Module B's context
        // but tries to free memory from Module A's heap
        // CRASH RISK when shared_ptr destructor runs!
    }
}

// ============================================================================
// EXAMPLE 5: DLL interface pattern (common real-world mistake)
// ============================================================================

// Simulating a DLL interface
namespace dll_interface {
    class ImageProcessor {
    public:
        virtual ~ImageProcessor() = default;
        virtual void process() = 0;
    };

    // BAD: Client must delete, but object is allocated in DLL
    ImageProcessor* createProcessor() {
        class ProcessorImpl : public ImageProcessor {
        public:
            void process() override {
                std::cout << "Processing in DLL...\n";
            }
        };
        return new ProcessorImpl();  // Allocated in DLL's heap
    }
}

// Client code
namespace client_code {
    void useProcessor() {
        dll_interface::ImageProcessor* proc = dll_interface::createProcessor();
        proc->process();

        // BAD: Client deletes object allocated in DLL!
        delete proc;  // CRASH RISK! Cross-module deallocation!
    }
}

// ============================================================================
// EXAMPLE 6: Array allocation/deallocation mismatch
// ============================================================================

namespace module_a {
    int* createArray(size_t size) {
        return new int[size];  // Array allocation in Module A
    }
}

namespace module_b {
    void cleanupArray(int* arr) {
        // BAD: Even if we use delete[] correctly, it's still wrong
        // because it's deallocating Module A's memory in Module B
        delete[] arr;  // CRASH RISK!

        // This would be even worse:
        // delete arr;  // DOUBLE CRASH RISK! Wrong deallocator AND module!
    }
}

// ============================================================================

int main() {
    std::cout << "=== Bad Examples: Cross-Module Memory Management ===\n\n";

    std::cout << "NOTE: These examples demonstrate dangerous patterns.\n";
    std::cout << "In real scenarios with actual DLLs/shared libraries,\n";
    std::cout << "these would likely crash or corrupt memory!\n\n";

    // Example 1
    {
        char* buf = module_a::createBuffer(100);
        // module_b::processAndCleanup(buf);  // Would crash in real DLL scenario
        delete[] buf;  // Cleanup in same module for this demo
    }

    // Example 2
    {
        auto* container = module_a::DataContainer::create(10);
        // module_b::processContainer(container);  // Would crash in real DLL scenario
        delete[] container->data;
        delete container;
    }

    // Example 5
    {
        // client_code::useProcessor();  // Would crash in real DLL scenario
    }

    std::cout << "\nSee good_example.cpp for correct approaches.\n";

    return 0;
}
