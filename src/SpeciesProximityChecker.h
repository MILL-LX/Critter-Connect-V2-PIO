#include "SpeciesCoordinates.h"

class SpeciesProximityChecker
{
public:
    enum Proximity
    {
        OUTSIDE_ZONES = 0,
        IN_ZONE_SPECIES_1,
        IN_ZONE_SPECIES_2
    };

    Proximity checkProximity(float lat, float lon);
};