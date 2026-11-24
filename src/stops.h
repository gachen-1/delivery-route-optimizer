// stops.h - delivery stop records, CSV loading and Haversine distance
#ifndef STOPS_H
#define STOPS_H

#include <string>
#include <vector>

// One delivery location. Index 0 of the stop list is always the depot.
struct Stop {
    int id;
    std::string name;
    double lat;   // latitude in degrees, -90..90
    double lon;   // longitude in degrees, -180..180
};

// Result of reading a stop file.
struct LoadResult {
    bool fileOpened;          // false if the file could not be opened
    std::vector<Stop> stops;  // valid rows, in file order
    int badRows;              // rows that were skipped
};

// Reads "id,name,latitude,longitude" rows (first line is a header).
// Invalid rows are skipped and reported with their line number.
LoadResult loadStops(const std::string& fileName);

// Great-circle distance in kilometres between two stops.
double haversine(const Stop& a, const Stop& b);

#endif
