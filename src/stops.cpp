
// stops.cpp - reading and validating the stop file
#include "stops.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

namespace {

// Removes spaces, tabs and a trailing '
' from both ends.
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r");
    return s.substr(start, end - start + 1);
}

// Converts text to a finite number and rejects trailing non-whitespace text.
bool parseDouble(const std::string& text, double& value) {
    if (text.empty()) return false;
    char* end = nullptr;
    errno = 0;
    value = std::strtod(text.c_str(), &end);
    return end != text.c_str() && *end == '\0' && errno != ERANGE &&
           std::isfinite(value);
}

bool parseInt(const std::string& text, int& value) {
    if (text.empty()) return false;
    char* end = nullptr;
    errno = 0;
    long v = std::strtol(text.c_str(), &end, 10);
    if (end == text.c_str() || *end != '\0' || errno == ERANGE ||
        v < std::numeric_limits<int>::min() || v > std::numeric_limits<int>::max()) {
        return false;
    }
    value = static_cast<int>(v);
    return true;
}

}  // namespace

LoadResult loadStops(const std::string& fileName) {
    LoadResult result;
    result.fileOpened = false;
    result.badRows = 0;

    std::ifstream in(fileName);
    if (!in) return result;
    result.fileOpened = true;

    std::string line;
    int lineNo = 0;
    if (std::getline(in, line)) lineNo = 1;  // skip the header row

    while (std::getline(in, line)) {
        lineNo++;
        if (trim(line).empty()) continue;    // ignore blank lines

        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, ',')) fields.push_back(trim(field));

        std::string reason;
        Stop s;
        if (fields.size() != 4) {
            reason = "expected 4 fields, found " + std::to_string(fields.size());
        } else if (!parseInt(fields[0], s.id)) {
            reason = "id is not a valid integer";
        } else if (fields[1].empty()) {
            reason = "name is empty";
        } else if (!parseDouble(fields[2], s.lat) || !parseDouble(fields[3], s.lon)) {
            reason = "latitude or longitude is not a finite number";
        } else if (s.lat < -90 || s.lat > 90 || s.lon < -180 || s.lon > 180) {
            reason = "coordinates out of range";
        }

        if (!reason.empty()) {
            std::cout << "  skipped line " << lineNo << ": " << reason << "\n";
            result.badRows++;
            continue;
        }
        s.name = fields[1];
        result.stops.push_back(s);
    }
    return result;
}

double haversine(const Stop& a, const Stop& b) {
    const double R = 6371.0;                       // Earth radius in km
    const double toRad = 3.14159265358979323846 / 180.0;
    double dLat = (b.lat - a.lat) * toRad;
    double dLon = (b.lon - a.lon) * toRad;
    double h = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(a.lat * toRad) * std::cos(b.lat * toRad) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    return 2 * R * std::asin(std::sqrt(h));
}
