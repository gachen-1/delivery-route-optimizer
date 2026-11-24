// route.cpp - Nearest Neighbor and route utilities
#include "route.h"

#include <fstream>
#include <iomanip>
#include <iostream>

std::vector<int> nearestNeighbor(double** D, int n) {
    std::vector<int> route(n + 1);
    bool* visited = new bool[n];
    for (int j = 0; j < n; j++) visited[j] = false;

    route[0] = 0;                      // start at the depot
    visited[0] = true;
    int cur = 0;
    for (int step = 1; step < n; step++) {
        int best = -1;
        for (int j = 1; j < n; j++) {  // scan every unvisited stop
            if (!visited[j] && (best == -1 || D[cur][j] < D[cur][best])) {
                best = j;
            }
        }
        route[step] = best;
        visited[best] = true;
        cur = best;
    }
    route[n] = 0;                      // return to the depot
    delete[] visited;
    return route;
}

std::vector<int> inputOrderRoute(int n) {
    std::vector<int> route(n + 1);
    for (int i = 0; i < n; i++) route[i] = i;
    route[n] = 0;
    return route;
}

double routeLength(double** D, const std::vector<int>& route) {
    double total = 0.0;
    for (size_t i = 0; i + 1 < route.size(); i++) {
        total += D[route[i]][route[i + 1]];
    }
    return total;
}

bool validateRoute(const std::vector<int>& route, int n) {
    if (n < 1 || static_cast<int>(route.size()) != n + 1) return false;
    if (route[0] != 0 || route[n] != 0) return false;
    bool* seen = new bool[n];
    for (int i = 0; i < n; i++) seen[i] = false;
    bool ok = true;
    for (int i = 0; i < n && ok; i++) {        // positions 0..n-1 hold each stop once
        int s = route[i];
        if (s < 0 || s >= n || seen[s]) ok = false;
        else seen[s] = true;
    }
    delete[] seen;
    return ok;
}

void printRoute(const std::vector<Stop>& stops, const std::vector<int>& route) {
    for (size_t i = 0; i < route.size(); i++) {
        std::cout << stops[route[i]].id;
        if (i + 1 < route.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

bool saveRoute(const std::string& fileName, const std::vector<Stop>& stops,
               const std::vector<int>& route, double** D) {
    std::ofstream out(fileName);
    if (!out) return false;
    out << "order,id,name,leg_km,cumulative_km\n";
    double cum = 0.0;
    out << std::fixed << std::setprecision(3);
    for (size_t i = 0; i < route.size(); i++) {
        double leg = (i == 0) ? 0.0 : D[route[i - 1]][route[i]];
        cum += leg;
        const Stop& s = stops[route[i]];
        out << i << "," << s.id << "," << s.name << "," << leg << "," << cum << "\n";
    }
    return true;
}
