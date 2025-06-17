#!/usr/bin/env python3

###########################################################################################################################################
# This script converts GeoJSON files containing species coordinates into a C header file format.
# It reads the coordinates from the GeoJSON files and outputs them in a format suitable for inclusion in a C program.
#
# Usage: python geojson_to_c.py <input_file1.geojson> [<input_file2.geojson> ...]
#
# The order of the species is determined by the order of the input files.
# The output will be printed to standard output, which can be redirected to a file.
#
# Examplefor usage this project: ./geojson_to_c.py Frog_data_FINAL.geojson Pigeon_data_FINAL.geojson > ../../src/app/SpeciesCoordinates.cpp
###########################################################################################################################################

import json
import sys

header = """#include "SpeciesCoordinates.h"

const SpeciesCoordinate speciesCoordinates[] = {
    // Test locations in Lisbon
    // MILL
    {-9.141010, 38.719576, 1},
    {-9.140070, 38.720580, 2},
    {-9.139885, 38.722155, 2},
    {-9.141329, 38.720367, 1},
    // TEST LOCATION 2
    {-9.156067344999594, 38.71925441818425, 1},
    {-9.155416, 38.719093, 2},

"""

footer = """};

const int numSpeciesCoordinates = sizeof(speciesCoordinates) / sizeof(speciesCoordinates[0]);
"""

if len(sys.argv) < 2:
    print("Usage: python geojson_to_c.py <input_file1.geojson> [<input_file2.geojson> ...]", file=sys.stderr)
    sys.exit(1)

print(header, end="")
for species_number, input_filename in enumerate(sys.argv[1:], start=1):
    with open(input_filename, "r") as f:
        data = json.load(f)
    features = data["features"]
    for feature in features:
        coords = feature["geometry"]["coordinates"]
        lon, lat = coords[0], coords[1]
        print(f"    {{{lon}, {lat}, {species_number}}},")
print(footer, end="")