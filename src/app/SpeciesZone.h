#pragma once

namespace SpeciesZone
{
    enum Zone
    {
        UNKNOWN_ZONE = -1,
        NON_SPECIES_ZONE,
        SPECIES_FROG_ZONE,
        SPECIES_PIGEON_ZONE,

    };

    Zone zoneForLatLon(float lat, float lon);
};