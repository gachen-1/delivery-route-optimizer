# Data

All files in this folder are **synthetic**. The coordinates are approximate locations of
Boston neighbourhoods chosen by the team (stops_10.csv) or random points inside the Boston
area generated with a fixed seed (stops_200.csv). No real customer data is used.

Format: a header row, then one stop per row as `id,name,latitude,longitude`.
The first data row is the depot.

| File | Rows | Purpose |
|---|---|---|
| stops_10.csv | depot + 9 stops | normal input, sample run 1 |
| stops_with_errors.csv | 10 data rows, 5 invalid | invalid-input handling, sample run 2 |
| stops_200.csv | depot + 199 stops | larger input for testing and timing |

When the dataset provided by Dr. Fatema Nafa for the EECE 7205 final project is added,
it will be placed in this folder and credited here.
