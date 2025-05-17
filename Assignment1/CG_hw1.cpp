#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

struct Point {
    double x, y, z;
    Point(double x1 = 0, double y1 = 0, double z1 = 0) : x(x1), y(y1), z(z1) {}
};

double computeBinomial(int n, int k) {
    if (k < 0 || k > n) return 0.0;
    if (k == 0 || k == n) return 1.0;

    vector<vector<double>> dp(n + 1, vector<double>(k + 1, 0.0));

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= min(i, k); ++j) {
            if (j == 0 || j == i)
                dp[i][j] = 1.0;
            else
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
        }
    }
    
    return dp[n][k];
}

Point computeBezier(const vector<Point>& ctrlPoints, double u) {
    int n = ctrlPoints.size() - 1; 
    Point result(0, 0, 0);

    for (int i = 0; i <= n; ++i) {
        double binomial = computeBinomial(n, i); 
        double bernstein = binomial * pow(1 - u, n - i) * pow(u, i); 

        // Weighted sum of control points
        result.x += bernstein * ctrlPoints[i].x;
        result.y += bernstein * ctrlPoints[i].y;
        result.z += bernstein * ctrlPoints[i].z;
    }

    return result;
}


int main(int argc, char* argv[]) {
    string filename = "control_points.txt";
    int n = 20;
    double radius = 0.1;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "-n" && i + 1 < argc) {
            n = stoi(argv[++i]);
        } else if (arg == "-r" && i + 1 < argc) {
            radius = stod(argv[++i]);
        }
    }

    // Read control points
    vector<Point> ctrlPoints;
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        double x, y, z;
        if (iss >> x >> y >> z) {
            ctrlPoints.emplace_back(x, y, z);
        }
    }
    if (ctrlPoints.empty()) {
        cerr << "Error: No control points read." << endl;
        return 1;
    }

    // Evaluate Bezier curve
    vector<Point> curvePoints;
    double du = 1.0 / n;
    for (int i = 0; i <= n; ++i) {
        double u = i * du;
        curvePoints.push_back(computeBezier(ctrlPoints, u));
    }

    // Generate Open Inventor output
    cout << "#Inventor V2.0 ascii\n";
    cout << "Separator {\n";

    // Control points as spheres
    cout << "  Separator {\n";
    cout << "    Material { }\n";
    for (const auto& pt : ctrlPoints) {
        cout << "    Separator {\n";
        cout << "      Translation { translation " 
                  << pt.x << " " << pt.y << " " << pt.z << " }\n";
        cout << "      Sphere { radius " << radius << " }\n";
        cout << "    }\n";
    }
    cout << "  }\n";

    // Bezier curve as LineSet
    cout << "  Separator {\n";
    cout << "    Material { }\n";
    cout << "    Coordinate3 {\n";
    cout << "      point [\n";
    for (size_t i = 0; i < curvePoints.size(); ++i) {
        const auto& p = curvePoints[i];
        cout << "        " << p.x << " " << p.y << " " << p.z;
        if (i < curvePoints.size() - 1) cout << ",";
        cout << "\n";
    }
    cout << "      ]\n";
    cout << "    }\n";
    cout << "    LineSet {\n";
    cout << "      numVertices [ " << curvePoints.size() << " ]\n";
    cout << "    }\n";
    cout << "  }\n";

    cout << "}\n";

    return 0;
}

