// Good: Nonmember function interface in the same namespace as the type

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

// Nonmember function in the same namespace - part of Point's interface
double distance(const Point& p1, const Point& p2) {
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return std::sqrt(dx * dx + dy * dy);
}

// Another nonmember function in the same namespace
Point midpoint(const Point& p1, const Point& p2) {
    return Point((p1.x() + p2.x()) / 2.0,
                 (p1.y() + p2.y()) / 2.0);
}

// Output operator - part of the interface
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x() << ", " << p.y() << ")";
}

} // namespace geometry

int main() {
    geometry::Point p1(0, 0);
    geometry::Point p2(3, 4);

    // ADL allows us to call without qualifying the function
    std::cout << "Distance: " << distance(p1, p2) << "\n";
    std::cout << "Midpoint: " << midpoint(p1, p2) << "\n";

    // operator<< also benefits from ADL
    std::cout << "Point 1: " << p1 << "\n";

    return 0;
}
