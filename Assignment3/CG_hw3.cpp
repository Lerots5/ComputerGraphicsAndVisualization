#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

struct Point3D {
    double x, y, z;
    Point3D operator+(const Point3D& p) const { return {x + p.x, y + p.y, z + p.z}; }
    Point3D operator*(double s) const { return {x * s, y * s, z * s}; }
    Point3D operator-(const Point3D& p) const { return {x - p.x, y - p.y, z - p.z}; }
    Point3D cross(const Point3D& p) const {
        return {y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x};
    }
    Point3D normalize() const {
        double len = sqrt(x * x + y * y + z * z);
        return {x / len, y / len, z / len};
    }
};

double bernstein(int n, int i, double t) {
    auto factorial = [](int x) {
        double f = 1;
        for (int j = 2; j <= x; j++) f *= j;
        return f;
    };
    return (factorial(n) / (factorial(i) * factorial(n - i))) * pow(t, i) * pow(1 - t, n - i);
}

vector<Point3D> bezierPatch(const vector<Point3D>& controlPoints, int num_u, int num_v) {
    double du = 1.0 / (num_u - 1);
    double dv = 1.0 / (num_v - 1);
    vector<Point3D> points;
    
    for (int i = 0; i < num_u; ++i) {
        double u = i * du;
        for (int j = 0; j < num_v; ++j) {
            double v = j * dv;
            Point3D p = {0, 0, 0};
            for (int m = 0; m < 4; ++m) {
                for (int n = 0; n < 4; ++n) {
                    p = p + controlPoints[m * 4 + n] * (bernstein(3, m, u) * bernstein(3, n, v));
                }
            }
            points.push_back(p);
        }
    }
    return points;
}

vector<Point3D> computeNormals(const vector<Point3D>& points, int num_u, int num_v) {
    vector<Point3D> normals(points.size(), {0, 0, 0});
    for (int i = 0; i < num_u - 1; ++i) {
        for (int j = 0; j < num_v - 1; ++j) {
            int p1 = i * num_v + j;
            int p2 = p1 + 1;
            int p3 = p1 + num_v;
            int p4 = p3 + 1;

            Point3D v1 = points[p2] - points[p1];
            Point3D v2 = points[p3] - points[p1];
            Point3D normal = v1.cross(v2).normalize();

            normals[p1] = normals[p1] + normal;
            normals[p2] = normals[p2] + normal;
            normals[p3] = normals[p3] + normal;
            normals[p4] = normals[p4] + normal;
        }
    }

    for (auto& normal : normals) {
        normal = normal.normalize();
    }

    return normals;
}

vector<Point3D> readControlPoints(const string& filename) {
    ifstream file(filename);
    vector<Point3D> points;
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Point3D p;
        iss >> p.x >> p.y >> p.z;
        points.push_back(p);
    }
    return points;
}

void writeInventor(const vector<Point3D>& points, const vector<Point3D>& normals, const vector<Point3D>& controlPoints, int num_u, int num_v, bool smoothShaded, float radius) {
    cout << "#Inventor V2.0 ascii\n\nSeparator {\n";
    // Bezier surface
    cout << "   Coordinate3 {\n       point [\n";
    for (size_t i = 0; i < points.size(); ++i) {
        cout << "           " << points[i].x << " " << points[i].y << " " << points[i].z;
        if (i != points.size() - 1) cout << ",";
        cout << "\n";
    }
    cout << "       ]\n   }\n";

    if (smoothShaded) {
        cout << "   Normal {\n       vector [\n";
        for (size_t i = 0; i < normals.size(); ++i) {
            cout << "           " << normals[i].x << " " << normals[i].y << " " << normals[i].z;
            if (i != normals.size() - 1) cout << ",";
            cout << "\n";
        }
        cout << "       ]\n   }\n";
    }

    cout << "   IndexedFaceSet {\n       coordIndex [\n";
    for (int i = 0; i < num_u - 1; i++) {
        for (int j = 0; j < num_v - 1; j++) {
            int p1 = i * num_v + j;
            int p2 = p1 + 1;
            int p3 = p1 + num_v;
            int p4 = p3 + 1;
            cout << "           " << p1 << ", " << p2 << ", " << p3 << ", -1,\n";
            cout << "           " << p2 << ", " << p4 << ", " << p3 << ", -1,\n";
        }
    }
    cout << "       ]\n   }\n";
    
    // Control points 
    cout << "   Separator {\n";
    cout << "      Material { diffuseColor 1 1 1 }\n";  
    for (const auto& p : controlPoints) {
        cout << "      Separator {\n";
        cout << "         Translation { translation " << p.x << " " << p.y << " " << p.z << " }\n";
        cout << "         Sphere { radius " << radius << " }\n";  // Sphere size (adjust radius as needed)
        cout << "      }\n";
    }
    cout << "   }\n";
    cout << "}\n";
}

int main(int argc, char* argv[]) {
    string filename = "patchPoints.txt";
    int num_u = 11, num_v = 11;
    float radius = 0.1f;
    bool smoothShaded = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) filename = argv[++i];
        else if (arg == "-u" && i + 1 < argc) num_u = stoi(argv[++i]);
        else if (arg == "-v" && i + 1 < argc) num_v = stoi(argv[++i]);
        else if (arg == "-r" && i + 1 < argc) radius = stof(argv[++i]);
        else if (arg == "-S") smoothShaded = true;
        else if (arg == "-F") smoothShaded = false;
    }
    
    vector<Point3D> controlPoints = readControlPoints(filename);
    vector<Point3D> patchPoints = bezierPatch(controlPoints, num_u, num_v);
    vector<Point3D> normals;

    if (smoothShaded) {
        normals = computeNormals(patchPoints, num_u, num_v);
    }

    writeInventor(patchPoints, normals, controlPoints, num_u, num_v, smoothShaded, radius);
    
    return 0;
}
