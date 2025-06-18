#include <atomic>
#include <memory>

#include "devices/ApplicationDevices.h"

#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"
#include "actions/MotorAction.h"

#include "app/SpeciesZone.h"

class GPSReceiverAction
{
public:
    GPSReceiverAction() : _periodicNeopixelAction(std::make_unique<PeriodicAction<NeoPixelAction>>(2000, UINT32_MAX, 1000, NeoPixel::StateColor::OK)),
                          _periodicMotor1Action(std::make_unique<PeriodicAction<MotorAction>>(10000UL, UINT32_MAX, 10000UL, ApplicationDevices::getInstance().getMotor1())),
                          _soundPlayer(ApplicationDevices::getInstance().getSoundPlayer()),
                          _neoPixel(ApplicationDevices::getInstance().getNeoPixel()),
                          _gpsReceiver(ApplicationDevices::getInstance().getGpsReceiver())

    {
    }

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
    bool isActive();
    void setActive(bool value);
    
private:
    std::atomic<bool> _is_running{false};
    SpeciesZone::Zone _previousZone = SpeciesZone::Zone::NON_SPECIES_ZONE;

    std::unique_ptr<PeriodicAction<NeoPixelAction>> _periodicNeopixelAction;
    std::unique_ptr<PeriodicAction<MotorAction>> _periodicMotor1Action;
    SoundPlayer &_soundPlayer;
    NeoPixel &_neoPixel;
    GPSReceiver &_gpsReceiver;
    std::atomic<bool> _isActive{false};

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};
