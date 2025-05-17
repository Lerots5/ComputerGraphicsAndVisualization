#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

struct Point {
    double x, y, z;
};

struct Tangent {
    double dx, dy, dz;
};

// Function to compute Catmull-Rom spline
Point catmullRomSpline(const Point& p0, const Point& p1, const Point& p2, const Point& p3, double u, double tension) {
    Point result;
    double u2 = u * u;
    double u3 = u2 * u;

    double m1x = (p2.x - p0.x) * (1 - tension) / 2;
    double m1y = (p2.y - p0.y) * (1 - tension) / 2;
    double m1z = (p2.z - p0.z) * (1 - tension) / 2;

    double m2x = (p3.x - p1.x) * (1 - tension) / 2;
    double m2y = (p3.y - p1.y) * (1 - tension) / 2;
    double m2z = (p3.z - p1.z) * (1 - tension) / 2;

    result.x = (2 * u3 - 3 * u2 + 1) * p1.x + (u3 - 2 * u2 + u) * m1x + (-2 * u3 + 3 * u2) * p2.x + (u3 - u2) * m2x;
    result.y = (2 * u3 - 3 * u2 + 1) * p1.y + (u3 - 2 * u2 + u) * m1y + (-2 * u3 + 3 * u2) * p2.y + (u3 - u2) * m2y;
    result.z = (2 * u3 - 3 * u2 + 1) * p1.z + (u3 - 2 * u2 + u) * m1z + (-2 * u3 + 3 * u2) * p2.z + (u3 - u2) * m2z;

    return result;
}

int main(int argc, char* argv[]) {
    std::string filename = "cpts_in.txt";
    int n = 11;
    double radius = 0.1;
    double tension = 0.0;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "-n" && i + 1 < argc) {
            n = std::stoi(argv[++i]);
        } else if (arg == "-r" && i + 1 < argc) {
            radius = std::stod(argv[++i]);
        } else if (arg == "-t" && i + 1 < argc) {
            tension = std::stod(argv[++i]);
        }
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    Tangent tangent1, tangent2;
    std::vector<Point> points;
    double dx, dy, dz, x, y, z;

    file >> tangent1.dx >> tangent1.dy >> tangent1.dz;
    file >> tangent2.dx >> tangent2.dy >> tangent2.dz;

    while (file >> x >> y >> z) {
        points.push_back({x, y, z});
    }

    file.close();

    double du = 1.0 / n;

    // Output Open Inventor format
    std::cout << "#Inventor V2.0 ascii\n\n";
    std::cout << "Separator {\n";
    std::cout << "  Material {\n";
    std::cout << "    diffuseColor 1 0 0\n";
    std::cout << "  }\n";
    std::cout << "  Coordinate3 {\n";
    std::cout << "    point [\n";

    for (size_t i = 0; i < points.size() - 1; i++) {
        const Point& p0 = (i == 0) ? Point{points[i].x - tangent1.dx, points[i].y - tangent1.dy, points[i].z - tangent1.dz} : points[i - 1];
        const Point& p1 = points[i];
        const Point& p2 = points[i + 1];
        const Point& p3 = (i == points.size() - 2) ? Point{points[i + 1].x + tangent2.dx, points[i + 1].y + tangent2.dy, points[i + 1].z + tangent2.dz} : points[i + 2];

        for (int j = 0; j <= n; j++) {
            double u = j * du;
            Point p = catmullRomSpline(p0, p1, p2, p3, u, tension);
            std::cout << "      " << p.x << " " << p.y << " " << p.z << ",\n";
        }
    }

    std::cout << "    ]\n";
    std::cout << "  }\n";
    std::cout << "  LineSet {\n";
    std::cout << "    numVertices [ ";
    for (size_t i = 0; i < points.size() - 1; i++) {
        std::cout << n + 1 << ", ";
    }
    std::cout << "]\n";
    std::cout << "  }\n";
    std::cout << "}\n";

    return 0;
}
