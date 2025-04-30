#include <atomic>

#include "devices/GPSReceiver.h"

class GPSReceiverAction
{
public:
    void performAction(uint32_t actionDurationMillis);

private:
    std::atomic<bool> _is_running{false};

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};