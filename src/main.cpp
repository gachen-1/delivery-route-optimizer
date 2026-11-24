// main.cpp - Delivery Route Optimizer, Iteration 3 prototype
// Usage: ./route_optimizer <stops.csv> [output_route.csv]
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "distance.h"
#include "route.h"
#include "stops.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./route_optimizer <stops.csv> [output_route.csv]\n";
        return 1;
    }
    std::string inFile = argv[1];

    std::cout << "Reading " << inFile << "\n";
    LoadResult data = loadStops(inFile);
    if (!data.fileOpened) {
        std::cout << "Error: cannot open file " << inFile << "\n";
        return 1;
    }
    int n = static_cast<int>(data.stops.size());
    std::cout << "Loaded " << n << " valid stops, skipped " << data.badRows << " invalid rows\n";
    if (n == 0) {
        std::cout << "No valid stops: nothing to plan.\n";
        return 1;
    }
    std::cout << "Depot: " << data.stops[0].id << " " << data.stops[0].name << "\n\n";

    double** D = buildDistanceMatrix(data.stops);

    auto t0 = std::chrono::steady_clock::now();
    std::vector<int> route = nearestNeighbor(D, n);
    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    double nnLen = routeLength(D, route);
    double baseLen = routeLength(D, inputOrderRoute(n));

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Nearest Neighbor route (stop IDs):\n  ";
    printRoute(data.stops, route);
    std::cout << "Route valid:            " << (validateRoute(route, n) ? "yes" : "NO") << "\n";
    std::cout << "Nearest Neighbor:       " << nnLen << " km\n";
    std::cout << "Input order (baseline): " << baseLen << " km\n";
    if (baseLen > 0) {
        std::cout << "Saving vs baseline:     " << std::setprecision(1)
                  << 100.0 * (baseLen - nnLen) / baseLen << " %\n" << std::setprecision(3);
    }
    std::cout << "Nearest Neighbor time:  " << ms << " ms\n";

    if (argc >= 3) {
        if (saveRoute(argv[2], data.stops, route, D))
            std::cout << "Route saved to " << argv[2] << "\n";
        else
            std::cout << "Error: cannot write " << argv[2] << "\n";
    }

    freeDistanceMatrix(D, n);
    return 0;
}
