// Bad: Nonmember function interface in different namespace

#include <iostream>
#include <cmath>

namespace geometry {

class Point {
private:
    double x_, y_;

public:
    Point(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }
};

} // namespace geometry

// Bad: Functions that are part of Point's interface are in global namespace
double distance(const geometry::Point& p1, const geometry::Point& p2) {
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return std::sqrt(dx * dx + dy * dy);
}

// Bad: Different namespace for related functionality
namespace utilities {
    geometry::Point midpoint(const geometry::Point& p1, const geometry::Point& p2) {
        return geometry::Point((p1.x() + p2.x()) / 2.0,
                              (p1.y() + p2.y()) / 2.0);
    }
}

int main() {
    geometry::Point p1(0, 0);
    geometry::Point p2(3, 4);

    // No ADL - must use fully qualified names or pollute with using declarations
    std::cout << "Distance: " << ::distance(p1, p2) << "\n";
    std::cout << "Midpoint: " << utilities::midpoint(p1, p2) << "\n";

    // Interface is scattered and harder to discover

    return 0;
}
