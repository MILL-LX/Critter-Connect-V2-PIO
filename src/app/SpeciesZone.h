#pragma once

namespace SpeciesZone
{
    enum Zone
    {
        NON_SPECIES_ZONE = 0,
        SPECIES_FROG_ZONE,
        SPECIES_PIGEON_ZONE
    };

    Zone zoneForLatLon(float lat, float lon);
};