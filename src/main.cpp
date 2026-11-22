// main.cpp - Delivery Route Optimizer, Iteration 3 prototype.
// Path: CSV file -> validated stops -> distance matrix -> Nearest Neighbor route
//       -> route validation -> printed route, per-leg distances and total.
// Exit codes: 0 success, 1 input error, 2 usage error, 3 internal error.
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "distance.h"
#include "route.h"
#include "stops.h"

namespace {

std::string label(const Stop& s) { return s.id + " (" + s.name + ")"; }

void printRoute(const std::string& inputPath, const std::vector<Stop>& stops,
                const DistanceMatrix& d, const std::vector<int>& route) {
    const std::size_t deliveries = stops.size() - 1;
    std::cout << "Delivery Route Optimizer - Iteration 3 prototype\n"
              << "Input file: " << inputPath << '\n'
              << "Stops loaded: " << stops.size() << " (1 depot, " << deliveries
              << (deliveries == 1 ? " delivery)\n" : " deliveries)\n")
              << "Algorithm: Nearest Neighbor\n";
    if (deliveries == 0) std::cout << "Note: no delivery stops, so the route stays at the depot.\n";

    std::cout << std::fixed << std::setprecision(3) << "\nLegs:\n";
    double running = 0.0;
    for (std::size_t i = 0; i + 1 < route.size(); ++i) {
        const double leg = d[route[i]][route[i + 1]];
        running += leg;
        std::cout << "  " << std::setw(3) << i + 1 << ". " << label(stops[route[i]]) << " -> "
                  << label(stops[route[i + 1]]) << ": " << leg << " km (total " << running << " km)\n";
    }

    std::cout << "\nRoute: ";
    for (std::size_t i = 0; i < route.size(); ++i) std::cout << (i ? " -> " : "") << stops[route[i]].name;
    std::cout << "\nVisited deliveries: " << deliveries << " of " << deliveries << '\n'
              << "Total straight-line distance: " << routeLength(d, route) << " km\n"
              << "Route validation: PASS\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: route_optimizer <stops.csv>\n";
        return 2;
    }
    std::vector<Stop> stops;
    std::string error;
    if (!loadStops(argv[1], stops, error)) {
        std::cerr << "Input error: " << error << '\n';
        return 1;
    }
    const DistanceMatrix d = buildDistanceMatrix(stops);
    const std::vector<int> route = nearestNeighbor(d);
    if (!validateRoute(route, static_cast<int>(stops.size()))) {
        std::cerr << "Internal error: Nearest Neighbor produced an invalid route\n";
        return 3;
    }
    printRoute(argv[1], stops, d, route);
    return 0;
}
