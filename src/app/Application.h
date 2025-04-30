#include <atomic>

#include "devices/GPSReceiver.h"

class Application
{
public:
    void run();

private:
    std::atomic<bool> _is_running{false};

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};