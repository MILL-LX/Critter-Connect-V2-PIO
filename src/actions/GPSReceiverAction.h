#include <atomic>

#include "devices/GPSReceiver.h"

class GPSReceiverAction
{
public:
    void performAction();

private:
    std::atomic<bool> _is_running{false};

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};