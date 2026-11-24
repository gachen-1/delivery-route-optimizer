# Delivery Route Optimizer

**Planning multi-stop delivery routes in C++ and comparing route-building algorithms**
EECE 7205 Fundamentals of Computer Engineering, Fall 2026, Final Project
Team: Jiachen Hou, Yuxuan Wei, Somber Xie

A delivery driver leaves a depot, visits every stop once and returns to the depot. Choosing
the visiting order is the Traveling Salesman Problem (TSP). This program reads the stops from
a CSV file, computes the great-circle (Haversine) distance between every pair of stops, builds
a route, checks that the route is valid, and reports its length and running time.

## Folder layout

```
src/            C++ source code
  stops.h/.cpp      Stop record, CSV loading with validation, Haversine distance
  distance.h/.cpp   n x n distance matrix (dynamically allocated 2D array)
  route.h/.cpp      Nearest Neighbor, route length, route validation, saving
  main.cpp          program entry point
data/           sample input files (synthetic, see data/README.md)
tests/          automated test program and test input files
```

## Build

From the repository root (macOS with Clang or Linux with g++):

```
g++ -std=c++17 -O2 -Wall src/*.cpp -o route_optimizer
g++ -std=c++17 -O2 -Wall tests/test_runner.cpp src/stops.cpp src/distance.cpp src/route.cpp -o test_runner
```

## Run

```
./route_optimizer data/stops_10.csv
./route_optimizer data/stops_10.csv output_route.csv     # also saves the route to a CSV file
./test_runner                                             # runs all tests, from the repository root
```

Input format: a header row, then `id,name,latitude,longitude` per row. The first data row is
the depot. Invalid rows are skipped and reported with their line number.

## Current progress (Iteration 3)

| Feature | Status |
|---|---|
| Load stops from CSV with row validation | Complete |
| Haversine distance matrix | Complete |
| Nearest Neighbor route construction | Complete |
| Route length, route validation, input-order baseline | Complete |
| Save route to CSV | Complete |
| Automated tests (10 cases) | Complete |
| Cheapest Insertion with our own doubly linked list | Planned for Iteration 4 |
| 2-opt improvement and exact backtracking for n <= 11 | Planned for Iteration 4 |
| Comparison table sorted with our own merge sort | Planned for Iteration 4 |
| Interactive menu | Planned for Iteration 4 |

## Known limitations

- Only one algorithm (Nearest Neighbor) is implemented so far. It is greedy, so on some inputs
  its route is longer than simply visiting the stops in file order
  (see `data/stops_with_errors.csv`: -3.2 %).
- Coordinates must be latitude and longitude; x/y coordinates are not supported yet.
- Names cannot contain commas, because the CSV reader splits on every comma.
- Single vehicle only; no capacity limits or time windows.
- The professor's dataset has not been added yet; all data is synthetic.

## Data source

All sample data is synthetic (see `data/README.md`).

## AI use

AI assistance is documented in each member's iteration report, following the course AI-use
policy. Every member has read, run and tested the submitted code.
