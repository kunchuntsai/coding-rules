// Good: Separate namespaces for unrelated functionality

#include <string>
#include <iostream>

// Graphics-related types and functions
namespace graphics {
    class Color {
        int r_, g_, b_;
    public:
        Color(int r, int g, int b) : r_(r), g_(g), b_(b) {}
        int red() const { return r_; }
        int green() const { return g_; }
        int blue() const { return b_; }
    };

    class Shape {
    protected:
        Color color_;
    public:
        Shape(const Color& c) : color_(c) {}
        virtual void draw() const = 0;
        virtual ~Shape() = default;
    };

    // Function that works with graphics types
    void render(const Shape& shape) {
        shape.draw();
    }
}

// File I/O related types and functions (separate concern)
namespace io {
    class FileHandle {
        std::string path_;
    public:
        explicit FileHandle(const std::string& path) : path_(path) {}
        const std::string& path() const { return path_; }
    };

    class Reader {
        FileHandle file_;
    public:
        explicit Reader(const FileHandle& f) : file_(f) {}
        std::string read() { return "file contents"; }
    };

    // Function that works with io types
    std::string loadFile(const FileHandle& file) {
        Reader r(file);
        return r.read();
    }
}

// Network related types (another separate concern)
namespace network {
    class Socket {
        int port_;
    public:
        explicit Socket(int port) : port_(port) {}
        int port() const { return port_; }
    };

    void connect(const Socket& sock) {
        std::cout << "Connecting to port " << sock.port() << "\n";
    }
}

int main() {
    graphics::Color red(255, 0, 0);
    io::FileHandle file("data.txt");
    network::Socket sock(8080);

    // Clear separation of concerns
    // Each namespace contains related functionality

    return 0;
}
