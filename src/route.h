// route.h - route construction and checking.
// A route is a vector of stop indices that starts and ends at depot index 0.
#ifndef ROUTE_H
#define ROUTE_H

#include <vector>

#include "distance.h"

// Iteration 3 core operation: from the current stop, always move to the closest
// unvisited delivery; ties go to the lower index. Returns n + 1 indices.
std::vector<int> nearestNeighbor(const DistanceMatrix& d);

// Sum of D[route[i]][route[i + 1]] over consecutive pairs.
double routeLength(const DistanceMatrix& d, const std::vector<int>& route);

// True if the route starts and ends at 0 and visits each of the n stops exactly once.
bool validateRoute(const std::vector<int>& route, int n);

#endif
