// test_runner.cpp - automated tests for the Iteration 3 prototype
// Build from the repository root:
//   g++ -std=c++17 -O2 -Wall tests/test_runner.cpp src/stops.cpp src/distance.cpp src/route.cpp -o test_runner
// Run from the repository root: ./test_runner
#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>

#include "../src/distance.h"
#include "../src/route.h"
#include "../src/stops.h"

int passed = 0, failed = 0;

void report(const std::string& name, const std::string& expected,
            const std::string& actual, bool ok) {
    std::cout << (ok ? "PASS  " : "FAIL  ") << name << "\n"
              << "      expected: " << expected << "\n"
              << "      actual:   " << actual << "\n";
    if (ok) passed++; else failed++;
}

std::string fmt(double x) {
    std::ostringstream s;
    s << std::fixed << std::setprecision(3) << x;
    return s.str();
}

std::string routeText(const std::vector<int>& r) {
    std::string t;
    for (size_t i = 0; i < r.size(); i++) t += std::to_string(r[i]) + (i + 1 < r.size() ? " " : "");
    return t;
}

// Loads a file, runs Nearest Neighbor and returns the route length (or -1 on failure).
double runNN(const std::string& file, int& n, bool& valid, std::vector<int>& route) {
    LoadResult data = loadStops(file);
    n = static_cast<int>(data.stops.size());
    valid = false;
    if (!data.fileOpened || n == 0) return -1;
    double** D = buildDistanceMatrix(data.stops);
    route = nearestNeighbor(D, n);
    valid = validateRoute(route, n);
    double len = routeLength(D, route);
    freeDistanceMatrix(D, n);
    return len;
}

int main() {
    int n; bool valid; std::vector<int> route;

    // T1 normal: four corners of a small square; NN must follow the perimeter.
    {
        double side = haversine({0, "a", 0.00, 0.00}, {1, "b", 0.01, 0.00});
        double len = runNN("tests/data/square.csv", n, valid, route);
        bool ok = valid && std::fabs(len - 4 * side) < 0.01;
        report("T1 normal: square, 4 stops",
               "valid route, length = 4 x side = " + fmt(4 * side) + " km",
               std::string(valid ? "valid" : "invalid") + ", route " + routeText(route) +
               ", length " + fmt(len) + " km", ok);
    }
    // T2 normal: 10 Boston stops; route valid and shorter than input order.
    {
        LoadResult data = loadStops("data/stops_10.csv");
        int m = static_cast<int>(data.stops.size());
        double** D = buildDistanceMatrix(data.stops);
        std::vector<int> r = nearestNeighbor(D, m);
        double nn = routeLength(D, r), base = routeLength(D, inputOrderRoute(m));
        bool ok = m == 10 && validateRoute(r, m) && nn < base;
        report("T2 normal: data/stops_10.csv",
               "10 stops, valid route, NN shorter than input order",
               std::to_string(m) + " stops, NN " + fmt(nn) + " km vs input order " + fmt(base) + " km", ok);
        freeDistanceMatrix(D, m);
    }
    // T3 boundary: depot only (n = 1).
    {
        double len = runNN("tests/data/depot_only.csv", n, valid, route);
        bool ok = n == 1 && valid && routeText(route) == "0 0" && len == 0.0;
        report("T3 boundary: depot only", "route 0 0, length 0.000 km",
               "route " + routeText(route) + ", length " + fmt(len) + " km", ok);
    }
    // T4 boundary: depot and one stop (n = 2).
    {
        LoadResult data = loadStops("tests/data/two_stops.csv");
        double d = haversine(data.stops[0], data.stops[1]);
        double len = runNN("tests/data/two_stops.csv", n, valid, route);
        bool ok = valid && routeText(route) == "0 1 0" && std::fabs(len - 2 * d) < 1e-9;
        report("T4 boundary: depot and one stop", "route 0 1 0, length 2 x D[0][1] = " + fmt(2 * d) + " km",
               "route " + routeText(route) + ", length " + fmt(len) + " km", ok);
    }
    // T5 boundary: two stops with identical coordinates.
    {
        LoadResult data = loadStops("tests/data/duplicate_coords.csv");
        double d = haversine(data.stops[1], data.stops[2]);
        runNN("tests/data/duplicate_coords.csv", n, valid, route);
        bool ok = d == 0.0 && valid;
        report("T5 boundary: identical coordinates", "distance 0.000 km between them, route valid",
               "distance " + fmt(d) + " km, route " + (valid ? "valid" : "invalid"), ok);
    }
    // T6 invalid: rows with missing fields, text, bad range, missing id and empty name.
    {
        std::cout << "(T6 loader messages)\n";
        LoadResult data = loadStops("tests/data/invalid_rows.csv");
        bool ok = data.fileOpened && data.stops.size() == 5 && data.badRows == 5;
        report("T6 invalid: 5 bad rows mixed with 5 good rows", "5 stops loaded, 5 rows skipped",
               std::to_string(data.stops.size()) + " stops loaded, " + std::to_string(data.badRows) + " rows skipped", ok);
    }
    // T7 invalid: file does not exist.
    {
        LoadResult data = loadStops("tests/data/no_such_file.csv");
        report("T7 invalid: missing file", "file not opened, no crash",
               data.fileOpened ? "file opened" : "file not opened", !data.fileOpened);
    }
    // T8 empty: header only, no stops.
    {
        LoadResult data = loadStops("tests/data/header_only.csv");
        bool ok = data.fileOpened && data.stops.empty() && data.badRows == 0;
        report("T8 empty: header only", "0 stops, 0 rows skipped",
               std::to_string(data.stops.size()) + " stops, " + std::to_string(data.badRows) + " rows skipped", ok);
    }
    // T9 validateRoute rejects broken routes.
    {
        bool a = !validateRoute({0, 1, 1, 0}, 3);      // stop 1 twice, stop 2 missing
        bool b = !validateRoute({1, 0, 2, 1}, 3);      // does not start at the depot
        bool c = !validateRoute({0, 1, 2}, 3);         // does not return to the depot
        report("T9 validateRoute on broken routes", "all three rejected",
               std::string(a && b && c ? "all three rejected" : "at least one accepted"), a && b && c);
    }
    // T10 larger input: 200 stops.
    {
        LoadResult data = loadStops("data/stops_200.csv");
        int m = static_cast<int>(data.stops.size());
        double** D = buildDistanceMatrix(data.stops);
        std::vector<int> r = nearestNeighbor(D, m);
        double nn = routeLength(D, r), base = routeLength(D, inputOrderRoute(m));
        bool ok = m == 200 && validateRoute(r, m) && nn < base;
        report("T10 larger input: data/stops_200.csv",
               "200 stops, valid route, NN shorter than input order",
               std::to_string(m) + " stops, NN " + fmt(nn) + " km vs input order " + fmt(base) + " km", ok);
        freeDistanceMatrix(D, m);
    }

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
