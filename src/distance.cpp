// distance.cpp - building and freeing the distance matrix
#include "distance.h"

double** buildDistanceMatrix(const std::vector<Stop>& stops) {
    int n = static_cast<int>(stops.size());
    double** D = new double*[n];
    for (int i = 0; i < n; i++) {
        D[i] = new double[n];
    }
    for (int i = 0; i < n; i++) {
        D[i][i] = 0.0;
        for (int j = i + 1; j < n; j++) {          // distance is symmetric
            double d = haversine(stops[i], stops[j]);
            D[i][j] = d;
            D[j][i] = d;
        }
    }
    return D;
}

void freeDistanceMatrix(double** D, int n) {
    if (D == nullptr) return;
    for (int i = 0; i < n; i++) delete[] D[i];
    delete[] D;
}
