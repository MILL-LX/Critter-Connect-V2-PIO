#include <atomic>

#include "devices/GPSReceiver.h"
#include "app/SpeciesZone.h"

class GPSReceiverAction
{
public:
    void performAction();

private:
    std::atomic<bool> _is_running{false};
    SpeciesZone::Zone _previousZone = SpeciesZone::Zone::NON_SPECIES_ZONE;
    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};