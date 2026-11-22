# Delivery Route Optimizer

**Planning multi-stop delivery routes in C++ and comparing route-building algorithms**
EECE 7205 Fundamentals of Computer Engineering, Fall 2026 — Final Project
Team: Jiachen Hou, Yuxuan Wei, Somber Xie

A delivery driver leaves a depot, must visit every delivery stop once, and returns to the depot. Choosing the visiting order is the Traveling Salesman Problem (TSP). This program reads the stops from a CSV file, computes the great-circle (Haversine) distance between every pair of stops, builds a route, checks that the route is valid, and prints the route with its per-leg and total distance.

Intended users: dispatchers at small local businesses who plan a daily route without commercial software, and students who want to see how routing heuristics behave on real inputs.

## Repository layout

```
src/
  stops.h / stops.cpp        Stop record, CSV parsing and validation (loadStops)
  distance.h / distance.cpp  Haversine distance and n x n distance matrix
  route.h / route.cpp        nearestNeighbor, routeLength, validateRoute
  main.cpp                   command-line entry point and output
data/                        synthetic sample inputs (see data/README.md)
tests/
  cases/                     one CSV file per test case (24 cases)
  run_tests.py               builds the program and runs every case
  make_large_case.py         regenerates tests/cases/large_200.csv (fixed seed)
```

## Build

Requires a C++17 compiler (g++ or clang++). From the repository root:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/*.cpp -o route_optimizer
```

**macOS troubleshooting.** If linking fails with `unknown architecture` from the newest Command Line Tools SDK, build against an older installed SDK, for example:

```sh
SDKROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX15.4.sdk g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/*.cpp -o route_optimizer
```

(`ls /Library/Developer/CommandLineTools/SDKs/` lists the installed SDKs.)

## Run

```sh
./route_optimizer data/sample_stops.csv
./route_optimizer data/boston_10_stops.csv
```

Exit codes: `0` success, `1` input error (message on standard error with the line number), `2` wrong number of arguments, `3` internal error (route failed validation).

### Input format

```
id,name,type,latitude,longitude
D,Depot,depot,42.3601,-71.0589
A,North Shop,delivery,42.3736,-71.1097
```

- The header must be exactly `id,name,type,latitude,longitude` (a UTF-8 byte-order mark and Windows line endings are accepted).
- The first data row is the single depot; every other row has type `delivery`.
- Latitude in [-90, 90], longitude in [-180, 180], IDs unique, names nonempty. A name containing a comma must be in double quotes.
- If any row is invalid the whole file is rejected and the first bad line is reported, so no delivery is silently left out of the route.

### Example output

```
Delivery Route Optimizer - Iteration 3 prototype
Input file: data/sample_stops.csv
Stops loaded: 4 (1 depot, 3 deliveries)
Algorithm: Nearest Neighbor

Legs:
    1. D (Depot) -> C (South Shop): 2.645 km (total 2.645 km)
    2. C (South Shop) -> B (East Shop): 3.684 km (total 6.329 km)
    3. B (East Shop) -> A (North Shop): 7.243 km (total 13.572 km)
    4. A (North Shop) -> D (Depot): 4.435 km (total 18.007 km)

Route: Depot -> South Shop -> East Shop -> North Shop -> Depot
Visited deliveries: 3 of 3
Total straight-line distance: 18.007 km
Route validation: PASS
```

## Test

```sh
python3 tests/run_tests.py
```

(prefix with the same `SDKROOT=...` setting on macOS if needed). The script compiles `src/*.cpp` and runs 24 cases: normal (3), boundary (5), invalid (11), empty (2), file format (2) and one larger input with 200 deliveries. Small cases are checked against hand-calculated distances (1 degree of arc = 111.195 km); the larger cases are checked against an independent Python implementation of Haversine + Nearest Neighbor. `--json results.json` saves the results.

## Current progress (Iteration 3)

- [x] Load and validate CSV input
- [x] Haversine distance matrix
- [x] Core operation: Nearest Neighbor route construction
- [x] Route length, route validation and readable per-leg output
- [x] Automated test suite (24 cases)
- [ ] Cheapest Insertion with the team's own doubly linked list (Iteration 4)
- [ ] 2-opt improvement and exact backtracking search for n ≤ 11 (Iteration 4)
- [ ] Menu, timing with `<chrono>`, comparison table, CSV export of results (Iteration 4)
- [ ] Instructor-provided delivery dataset with source credit (Iteration 4)

## Known limitations

- Distances are straight-line great-circle distances, not road distances or driving times.
- Nearest Neighbor is a greedy heuristic: it does not guarantee the shortest route, and it often leaves long legs for the end of the route.
- One depot, one vehicle; no capacity limits or delivery time windows.
- The whole file is rejected at the first invalid row; multiline quoted CSV fields are not supported.
- The n x n distance matrix needs about 8·n² bytes (about 80 MB at n = 3,200).
- All current inputs are synthetic.
