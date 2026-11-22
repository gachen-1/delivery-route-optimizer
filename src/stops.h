// stops.h - stop records and CSV loading for the Delivery Route Optimizer.
#ifndef STOPS_H
#define STOPS_H

#include <string>
#include <vector>

// One location from the input file. Index 0 of the stop list is always the depot.
struct Stop {
    std::string id, name, type;      // type is "depot" or "delivery"
    double latitude = 0.0;           // degrees, [-90, 90]
    double longitude = 0.0;          // degrees, [-180, 180]
};

// Reads id,name,type,latitude,longitude rows from a CSV file.
// Returns false and sets `error` (with a line number when possible) if the
// file cannot be used; the whole file is rejected so no delivery is silently dropped.
bool loadStops(const std::string& path, std::vector<Stop>& stops, std::string& error);

#endif
