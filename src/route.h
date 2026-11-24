// route.h - building, checking, printing and saving a route
#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "stops.h"

// A route is a list of stop indices that starts and ends at the depot (index 0).
// For n stops it has n + 1 entries.

// Builds a route by always moving to the closest unvisited stop.
std::vector<int> nearestNeighbor(double** D, int n);

// Visits the stops in the order they appear in the file (baseline for comparison).
std::vector<int> inputOrderRoute(int n);

// Total distance of a route in km.
double routeLength(double** D, const std::vector<int>& route);

// True if the route starts and ends at 0 and visits every stop exactly once.
bool validateRoute(const std::vector<int>& route, int n);

// Prints the route as stop IDs.
void printRoute(const std::vector<Stop>& stops, const std::vector<int>& route);

// Writes the route to a CSV file. Returns false if the file cannot be written.
bool saveRoute(const std::string& fileName, const std::vector<Stop>& stops,
               const std::vector<int>& route, double** D);

#endif
