#include <Arduino.h>

#include "SpeciesZone.h"
#include "SpeciesCoordinates.h"

const float earthRadius = 6371000;
const float zoneRadius = 35.0; // Radius in meters for species zones

float toRadians(float degree)
{
  return degree * (PI / 180);
}

float haversineDistance(float lat1, float lon1, float lat2, float lon2)
{
  // Convert latitude and longitude from degrees to radians
  lat1 = toRadians(lat1);
  lon1 = toRadians(lon1);
  lat2 = toRadians(lat2);
  lon2 = toRadians(lon2);

  // Difference in coordinates
  float dLat = lat2 - lat1;
  float dLon = lon2 - lon1;

  // Haversine formula
  float a = sin(dLat / 2) * sin(dLat / 2) +
            cos(lat1) * cos(lat2) *
                sin(dLon / 2) * sin(dLon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));

  // Distance in meters
  float distance = earthRadius * c;

  return distance;
}

SpeciesZone::Zone SpeciesZone::zoneForLatLon(float lat, float lon)
{
  uint8_t matching_species = 0;

  for (int i = 0; !matching_species && i < numSpeciesCoordinates; i++)
  {
    float distance = haversineDistance(speciesCoordinates[i].latitude, speciesCoordinates[i].longitude, lat, lon);
    // MODEBUG: Uncomment the following line to enable debug output
    // Serial.printf("Checking species %d at (%f, %f) against (%f, %f): distance = %f meters\n",
    //               speciesCoordinates[i].species, speciesCoordinates[i].latitude, speciesCoordinates[i].longitude, lat, lon, distance);
    if (distance <= zoneRadius)
    {
      Serial.printf("Found matching species %d at (%f, %f) within radius %f meters\n",
                    speciesCoordinates[i].species, speciesCoordinates[i].latitude, speciesCoordinates[i].longitude, zoneRadius);
      matching_species = speciesCoordinates[i].species;
      break;
    }
  }

  // Cast the matching species to the Zone enum. Slightly dirty but flexible when adding new species.
  // This assumes that species are defined in a way that they map directly to the Zone enum.
  SpeciesZone::Zone zone = static_cast<SpeciesZone::Zone>(matching_species);
  return zone;
}