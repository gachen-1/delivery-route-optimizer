# Sample input data

All files here are **synthetic** and contain no personal records. Coordinates are approximate public neighborhood centers in the Boston area or simple test points; names are invented labels.

| File | Stops | Purpose |
|---|---|---|
| `sample_stops.csv` | 1 depot + 3 deliveries | Small normal run (sample run 1 in the report) |
| `boston_10_stops.csv` | 1 depot + 10 deliveries | Larger normal run (sample run 2 in the report) |

The real delivery dataset named in the Iteration 2 proposal (provided by the course instructor) has not been added yet; it will be added with its source credit in Iteration 4.

Format: header `id,name,type,latitude,longitude`; the first data row is the single depot (`type` = `depot`), every other row is a `delivery`. Latitude must be in [-90, 90], longitude in [-180, 180], IDs must be unique, and a name containing a comma must be in double quotes.
