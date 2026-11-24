// distance.h - n x n distance matrix stored as our own 2D array
#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>
#include "stops.h"

// Allocates an n x n matrix with D[i][j] = distance from stop i to stop j (km).
double** buildDistanceMatrix(const std::vector<Stop>& stops);

// Releases the memory allocated by buildDistanceMatrix.
void freeDistanceMatrix(double** D, int n);

#endif
