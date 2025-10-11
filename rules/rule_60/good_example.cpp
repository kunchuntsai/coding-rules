// Good: Proper memory management across module boundaries
// This demonstrates the CORRECT way to handle memory in module interfaces

#include <iostream>
#include <string>
#include <memory>
#include <cstring>

// ============================================================================
// EXAMPLE 1: Factory and Destroyer Pattern
// ============================================================================

// Module A's interface
namespace module_a {
    class Buffer {
    private:
        char* data_;
        size_t size_;

        Buffer(size_t size) : data_(new char[size]), size_(size) {}

    public:
        // GOOD: Factory function - module allocates
        static Buffer* create(size_t size) {
            return new Buffer(size);
        }

        // GOOD: Destroyer function - same module deallocates
        static void destroy(Buffer* buffer) {
            delete buffer;  // Safe: same module's heap
        }

        ~Buffer() {
            delete[] data_;  // Internal cleanup in same module
        }

        size_t size() const { return size_; }
        char* data() { return data_; }
    };
}

// Module B's usage
namespace module_b {
    void useBuffer() {
        // Create via factory
        module_a::Buffer* buffer = module_a::Buffer::create(1024);

        // Use the buffer
        std::cout << "Buffer size: " << buffer->size() << "\n";

        // GOOD: Destroy via same module's destroyer
        module_a::Buffer::destroy(buffer);
    }
}

// ============================================================================
// EXAMPLE 2: Opaque Pointer Pattern (PIMPL)
// ============================================================================

namespace module_a {
    // Forward declaration - implementation details hidden
    struct DataImpl;

    class Data {
    private:
        DataImpl* impl_;  // Opaque pointer

    public:
        Data();   // Implemented in module A
        ~Data();  // Implemented in module A (handles impl_ deallocation)

        void setValue(int value);
        int getValue() const;

        // Prevent copying (would require module-aware copy)
        Data(const Data&) = delete;
        Data& operator=(const Data&) = delete;
    };

    // Implementation (would be in .cpp file)
    struct DataImpl {
        int value;
    };

    Data::Data() : impl_(new DataImpl{0}) {}
    Data::~Data() { delete impl_; }  // Same module handles deallocation

    void Data::setValue(int value) { impl_->value = value; }
    int Data::getValue() const { return impl_->value; }
}

// ============================================================================
// EXAMPLE 3: RAII Wrapper with Custom Deleter
// ============================================================================

namespace module_a {
    // Resource type
    struct Resource {
        int id;
        char buffer[256];
    };

    // GOOD: Module provides creation and deletion
    Resource* createResource(int id) {
        Resource* r = new Resource;
        r->id = id;
        std::sprintf(r->buffer, "Resource %d", id);
        return r;
    }

    void destroyResource(Resource* r) {
        delete r;  // Same module deallocates
    }

    // GOOD: Provide a unique_ptr typedef with custom deleter
    struct ResourceDeleter {
        void operator()(Resource* r) const {
            destroyResource(r);
        }
    };

    using ResourcePtr = std::unique_ptr<Resource, ResourceDeleter>;

    ResourcePtr makeResource(int id) {
        return ResourcePtr(createResource(id));
    }
}

// Module B usage
namespace module_b {
    void useResource() {
        // GOOD: RAII with custom deleter ensures proper cleanup
        auto resource = module_a::makeResource(42);

        std::cout << "Using: " << resource->buffer << "\n";

        // Automatic cleanup via custom deleter when scope exits
    }
}

// ============================================================================
// EXAMPLE 4: Preallocated Buffer Pattern
// ============================================================================

namespace module_a {
    // GOOD: Caller provides buffer, module just uses it
    bool processData(char* outputBuffer, size_t bufferSize, const char* input) {
        size_t inputLen = std::strlen(input);
        if (inputLen >= bufferSize) {
            return false;  // Buffer too small
        }

        // Process data into caller's buffer
        std::strcpy(outputBuffer, input);
        for (size_t i = 0; i < inputLen; ++i) {
            outputBuffer[i] = std::toupper(outputBuffer[i]);
        }

        return true;
    }

    // Alternative: Query size first, then process
    size_t getRequiredBufferSize(const char* input) {
        return std::strlen(input) + 1;
    }
}

// Module B usage
namespace module_b {
    void useProcessData() {
        const char* input = "hello world";

        // Allocate buffer in Module B
        size_t size = module_a::getRequiredBufferSize(input);
        char* buffer = new char[size];

        // Module A uses but doesn't own the buffer
        if (module_a::processData(buffer, size, input)) {
            std::cout << "Processed: " << buffer << "\n";
        }

        // Deallocate in Module B (same module)
        delete[] buffer;
    }
}

// ============================================================================
// EXAMPLE 5: Interface with Factory (DLL-safe pattern)
// ============================================================================

namespace dll_interface {
    class IImageProcessor {
    public:
        virtual ~IImageProcessor() = default;
        virtual void process() = 0;
        virtual void release() = 0;  // GOOD: Self-delete method
    };

    // GOOD: Object deletes itself (same module)
    class ImageProcessorImpl : public IImageProcessor {
    public:
        void process() override {
            std::cout << "Processing image in DLL...\n";
        }

        void release() override {
            delete this;  // GOOD: Self-deletion in same module
        }
    };

    // Factory function
    IImageProcessor* createProcessor() {
        return new ImageProcessorImpl();
    }
}

// Client code
namespace client_code {
    void useProcessor() {
        dll_interface::IImageProcessor* proc = dll_interface::createProcessor();
        proc->process();

        // GOOD: Call release() instead of delete
        proc->release();  // Object deletes itself in correct module
    }
}

// ============================================================================
// EXAMPLE 6: Container with Module-Safe Memory
// ============================================================================

namespace module_a {
    class SafeContainer {
    private:
        int* data_;
        size_t size_;

    public:
        SafeContainer(size_t size) : data_(new int[size]), size_(size) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = 0;
            }
        }

        ~SafeContainer() {
            delete[] data_;  // Same module handles memory
        }

        // Prevent copying (would need deep copy consideration)
        SafeContainer(const SafeContainer&) = delete;
        SafeContainer& operator=(const SafeContainer&) = delete;

        // Allow moving (transfers ownership safely)
        SafeContainer(SafeContainer&& other) noexcept
            : data_(other.data_), size_(other.size_) {
            other.data_ = nullptr;
            other.size_ = 0;
        }

        void set(size_t index, int value) {
            if (index < size_) data_[index] = value;
        }

        int get(size_t index) const {
            return (index < size_) ? data_[index] : 0;
        }

        size_t size() const { return size_; }
    };
}

// ============================================================================

int main() {
    std::cout << "=== Good Examples: Module-Safe Memory Management ===\n\n";

    // Example 1: Factory/Destroyer
    std::cout << "Example 1: Factory/Destroyer Pattern\n";
    module_b::useBuffer();
    std::cout << "\n";

    // Example 2: Opaque Pointer
    std::cout << "Example 2: Opaque Pointer (PIMPL)\n";
    {
        module_a::Data data;
        data.setValue(42);
        std::cout << "Data value: " << data.getValue() << "\n";
    }
    std::cout << "\n";

    // Example 3: RAII with Custom Deleter
    std::cout << "Example 3: RAII with Custom Deleter\n";
    module_b::useResource();
    std::cout << "\n";

    // Example 4: Preallocated Buffer
    std::cout << "Example 4: Preallocated Buffer\n";
    module_b::useProcessData();
    std::cout << "\n";

    // Example 5: Self-releasing Interface
    std::cout << "Example 5: Self-releasing Interface\n";
    client_code::useProcessor();
    std::cout << "\n";

    // Example 6: RAII Container
    std::cout << "Example 6: RAII Container\n";
    {
        module_a::SafeContainer container(10);
        container.set(0, 100);
        std::cout << "Container[0] = " << container.get(0) << "\n";
    }
    std::cout << "\n";

    std::cout << "All examples use module-safe memory management!\n";

    return 0;
}
