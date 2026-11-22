// distance.cpp - Haversine formula and distance matrix.
#include "distance.h"

#include <algorithm>
#include <cmath>

double haversineKm(const Stop& a, const Stop& b) {
    const double pi = std::acos(-1.0);
    const double radiusKm = 6371.0088;  // mean Earth radius
    const double toRad = pi / 180.0;
    const double lat1 = a.latitude * toRad;
    const double lat2 = b.latitude * toRad;
    const double dLat = (b.latitude - a.latitude) * toRad;
    const double dLon = (b.longitude - a.longitude) * toRad;
    const double h = std::sin(dLat / 2) * std::sin(dLat / 2) +
                     std::cos(lat1) * std::cos(lat2) * std::sin(dLon / 2) * std::sin(dLon / 2);
    // Clamp h to [0, 1] so rounding error never makes asin's argument invalid.
    return 2 * radiusKm * std::asin(std::sqrt(std::max(0.0, std::min(1.0, h))));
}

DistanceMatrix buildDistanceMatrix(const std::vector<Stop>& stops) {
    const std::size_t n = stops.size();
    DistanceMatrix d(n, std::vector<double>(n, 0.0));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = i + 1; j < n; ++j)
            d[i][j] = d[j][i] = haversineKm(stops[i], stops[j]);
    return d;
}
