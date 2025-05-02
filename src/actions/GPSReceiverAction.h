#include <atomic>
#include <memory> // Include for std::unique_ptr

#include "devices/GPSReceiver.h"
#include "devices/NeoPixel.h"
#include "devices/SoundPlayer.h"

#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"

#include "app/SpeciesZone.h"

class GPSReceiverAction
{
public:
    GPSReceiverAction()        : _periodicNeopixelAction(std::make_unique<PeriodicAction<NeoPixelAction>>(2000, UINT32_MAX, 1000, NeoPixel::StateColor::OK)){}

    // Destructor is still needed to explicitly stop the periodic action before
    // its unique_ptr goes out of scope and deletes the object.
    ~GPSReceiverAction()
    {
        if (_periodicNeopixelAction)
        {
            _periodicNeopixelAction->stop();
            // Give the task some time to stop cleanly if needed,
            // though ideally stop() would handle synchronization.
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        // No need for delete calls; unique_ptr handles it automatically.
    }

    // Prevent copying and assignment, as unique_ptr is not copyable
    GPSReceiverAction(const GPSReceiverAction &) = delete;
    GPSReceiverAction &operator=(const GPSReceiverAction &) = delete;

    // Allow moving if necessary (optional, but good practice)
    GPSReceiverAction(GPSReceiverAction &&) = default;
    GPSReceiverAction &operator=(GPSReceiverAction &&) = default;

    void performAction();

private:
    std::atomic<bool> _is_running{false};
    SpeciesZone::Zone _previousZone = SpeciesZone::Zone::NON_SPECIES_ZONE;

    std::unique_ptr<PeriodicAction<NeoPixelAction>> _periodicNeopixelAction;

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};
