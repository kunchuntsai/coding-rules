// Bad: Mixing unrelated types and functions in the same namespace

#include <string>
#include <iostream>

namespace myapp {
    // Graphics stuff
    class Color {
        int r_, g_, b_;
    public:
        Color(int r, int g, int b) : r_(r), g_(g), b_(b) {}
    };

    class Shape {
        Color color_;
    public:
        Shape(const Color& c) : color_(c) {}
    };

    // File I/O stuff mixed in (unrelated!)
    class FileHandle {
        std::string path_;
    public:
        explicit FileHandle(const std::string& path) : path_(path) {}
    };

    class Reader {
        FileHandle file_;
    public:
        explicit Reader(const FileHandle& f) : file_(f) {}
    };

    // Network stuff also mixed in (unrelated!)
    class Socket {
        int port_;
    public:
        explicit Socket(int port) : port_(port) {}
    };

    // Now we have name collision risks
    // What if we want a Color reader? Or a graphics Renderer?
    // The namespace is polluted with unrelated concerns

    void render(const Shape& shape) { /* ... */ }
    void connect(const Socket& sock) { /* ... */ }
    std::string read(const FileHandle& file) { /* ... */ return ""; }
}

int main() {
    // Everything is in one namespace
    // Hard to see dependencies
    // Risk of name conflicts
    // Tight coupling of unrelated concepts

    myapp::Color red(255, 0, 0);
    myapp::FileHandle file("data.txt");
    myapp::Socket sock(8080);

    return 0;
}
