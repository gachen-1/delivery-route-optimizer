// stops.cpp - CSV parsing and validation of stop records.
#include "stops.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <unordered_set>

namespace {

std::string trim(const std::string& s) {
    const auto a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    const auto b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Splits one CSV line into trimmed fields. Double-quoted fields may contain
// commas, and "" inside quotes is an escaped quote. Returns false on an
// unterminated quote.
bool parseCsvRow(const std::string& line, std::vector<std::string>& fields) {
    fields.clear();
    std::string field;
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (c == '"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                quoted = !quoted;
            }
        } else if (c == ',' && !quoted) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    if (quoted) return false;
    fields.push_back(trim(field));
    return true;
}

// Accepts the whole field as a finite number, e.g. rejects "42.1abc", "", "nan".
bool parseCoordinate(const std::string& text, double& value) {
    try {
        std::size_t used = 0;
        value = std::stod(text, &used);
        return used == text.size() && std::isfinite(value);
    } catch (...) {
        return false;
    }
}

std::string lineError(int lineNumber, const std::string& message) {
    return "Line " + std::to_string(lineNumber) + ": " + message;
}

}  // namespace

bool loadStops(const std::string& path, std::vector<Stop>& stops, std::string& error) {
    stops.clear();
    std::error_code ec;
    if (std::filesystem::is_directory(path, ec)) {
        error = "Input path is a directory, not a CSV file: " + path;
        return false;
    }
    std::ifstream in(path);
    if (!in) {
        error = "Cannot open input file: " + path;
        return false;
    }

    std::string line;
    if (!std::getline(in, line)) {
        error = "Input file is empty";
        return false;
    }
    // Spreadsheet programs often save "CSV UTF-8" with a byte-order mark.
    if (line.rfind("\xEF\xBB\xBF", 0) == 0) line.erase(0, 3);

    std::vector<std::string> fields;
    const std::vector<std::string> header{"id", "name", "type", "latitude", "longitude"};
    if (!parseCsvRow(line, fields) || fields != header) {
        error = lineError(1, "expected header id,name,type,latitude,longitude");
        return false;
    }

    int lineNumber = 1;
    int depots = 0;
    std::unordered_set<std::string> seenIds;  // average O(1) duplicate check
    while (std::getline(in, line)) {
        ++lineNumber;
        if (trim(line).empty()) continue;
        if (!parseCsvRow(line, fields) || fields.size() != 5) {
            error = lineError(lineNumber, "expected 5 CSV fields");
            return false;
        }
        Stop s;
        s.id = fields[0];
        s.name = fields[1];
        s.type = fields[2];
        if (s.id.empty() || s.name.empty() || (s.type != "depot" && s.type != "delivery")) {
            error = lineError(lineNumber, "id/name must be nonempty and type must be depot or delivery");
            return false;
        }
        if (!parseCoordinate(fields[3], s.latitude) || !parseCoordinate(fields[4], s.longitude) ||
            s.latitude < -90 || s.latitude > 90 || s.longitude < -180 || s.longitude > 180) {
            error = lineError(lineNumber, "invalid latitude or longitude");
            return false;
        }
        if (!seenIds.insert(s.id).second) {
            error = lineError(lineNumber, "duplicate id " + s.id);
            return false;
        }
        if (s.type == "depot") {
            if (depots > 0) {
                error = lineError(lineNumber, "second depot found; exactly one depot is allowed");
                return false;
            }
            if (!stops.empty()) {
                error = lineError(lineNumber, "the depot must be the first data row");
                return false;
            }
            ++depots;
        }
        stops.push_back(s);
    }

    if (stops.empty()) {
        error = "Input has a header but no stop records";
        return false;
    }
    if (depots == 0) {
        error = "No depot found; the first data row must be the depot";
        return false;
    }
    return true;
}
