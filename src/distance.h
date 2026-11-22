// distance.h - great-circle distances between stops.
#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>

#include "stops.h"

// D[i][j] = straight-line (great-circle) distance in km from stop i to stop j.
using DistanceMatrix = std::vector<std::vector<double>>;

double haversineKm(const Stop& a, const Stop& b);

// Builds the symmetric n x n matrix once, so every later lookup is O(1).
DistanceMatrix buildDistanceMatrix(const std::vector<Stop>& stops);

#endif
