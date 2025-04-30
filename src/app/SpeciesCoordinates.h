#pragma once

struct SpeciesCoordinate
{
    float longitude;
    float latitude;
    int species; 
};

extern const SpeciesCoordinate speciesCoordinates[];
extern const int numSpeciesCoordinates;
