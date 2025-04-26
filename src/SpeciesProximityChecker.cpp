#include <Arduino.h>

#include "SpeciesProximityChecker.h"
#include "SpeciesCoordinates.h"

const float earthRadius = 6371000;

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

SpeciesProximityChecker::Proximity SpeciesProximityChecker::checkProximity(float lat, float lon)
{
  bool matching_species = 0;

  for (int i = 0; !matching_species && i < numSpeciesCoordinates; i++)
  {
    float distance = haversineDistance(speciesCoordinates[i].latitude, speciesCoordinates[i].longitude, lat, lon);
    if(distance <= 40.0)
      matching_species = speciesCoordinates[i].species;
  }

  if (!matching_species) 
    return SpeciesProximityChecker::OUTSIDE_ZONES;
  else
    return (matching_species == 1) ? SpeciesProximityChecker::IN_ZONE_SPECIES_1 : SpeciesProximityChecker::IN_ZONE_SPECIES_2;
}