// route.cpp - Nearest Neighbor construction, route length and validation.
#include "route.h"

#include <algorithm>

// Follows NEARESTNEIGHBOR(D, n) from the Iteration 2 proposal (comments give its line numbers).
std::vector<int> nearestNeighbor(const DistanceMatrix& d) {
    const int n = static_cast<int>(d.size());
    std::vector<bool> visited(n, false);              // line 2
    std::vector<int> route{0};                        // line 3: route[0] <- 0
    visited[0] = true;
    int current = 0;
    for (int step = 1; step < n; ++step) {            // line 4
        int best = -1;                                // line 5
        for (int j = 1; j < n; ++j)                   // line 6
            if (!visited[j] && (best == -1 || d[current][j] < d[current][best]))  // line 7
                best = j;                             // line 8
        route.push_back(best);                        // line 9
        visited[best] = true;
        current = best;
    }
    route.push_back(0);                               // line 10: return to the depot
    return route;                                     // line 11
}

double routeLength(const DistanceMatrix& d, const std::vector<int>& route) {
    double total = 0.0;
    for (std::size_t i = 0; i + 1 < route.size(); ++i) total += d[route[i]][route[i + 1]];
    return total;
}

bool validateRoute(const std::vector<int>& route, int n) {
    if (n < 1 || route.size() != static_cast<std::size_t>(n + 1) || route.front() != 0 || route.back() != 0)
        return false;
    std::vector<int> counts(n, 0);
    for (int i = 0; i < n; ++i) {  // route[n] is the return to the depot, checked above
        const int v = route[i];
        if (v < 0 || v >= n) return false;
        ++counts[v];
    }
    return std::all_of(counts.begin(), counts.end(), [](int c) { return c == 1; });
}
