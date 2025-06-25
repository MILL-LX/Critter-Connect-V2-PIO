#include <atomic>
#include <memory>

#include "devices/ApplicationDevices.h"

#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"
#include "actions/MotorAction.h"
#include "actions/SoundButtonAction.h"
#include "actions/VibratingMotorAction.h"

#include "app/SpeciesZone.h"

class GPSReceiverAction
{
public:
    GPSReceiverAction() : _periodicNeopixelAction(std::make_unique<PeriodicAction<NeoPixelAction>>(2000, UINT32_MAX, 1000, NeoPixel::StateColor::OK)),
                          _soundPlayer(ApplicationDevices::getInstance().getSoundPlayer()),
                          _soundButtonAction_frog(std::make_unique<SoundButtonAction>(SoundPlayer::Sound::SPECIES_FROG)),
                          _soundButtonAction_pigeon(std::make_unique<SoundButtonAction>(SoundPlayer::Sound::SPECIES_PIGEON)),
                          _periodicMotorAction_frog_long(std::make_unique<PeriodicAction<MotorAction>>(60000, 1, 4,
                                                                                                       ApplicationDevices::getInstance().getMotorFrog())),
                          _periodicMotorAction_frog_short(std::make_unique<PeriodicAction<MotorAction>>(60000, UINT32_MAX, 2,
                                                                                                        ApplicationDevices::getInstance().getMotorFrog())),
                          _periodicMotorAction_pigeon_long(std::make_unique<PeriodicAction<MotorAction>>(60000, 1, 4,
                                                                                                         ApplicationDevices::getInstance().getMotorPigeon())),
                          _periodicMotorAction_pigeon_short(std::make_unique<PeriodicAction<MotorAction>>(60000, UINT32_MAX, 2,
                                                                                                          ApplicationDevices::getInstance().getMotorPigeon())),
                          _periodicVibratingMotorAction_long(std::make_unique<PeriodicAction<VibratingMotorAction>>(60000, 1, 5000, 2500,
                                                                                                                 ApplicationDevices::getInstance().getVibratingMotor())),
                          _periodicVibratingMotorAction_short(std::make_unique<PeriodicAction<VibratingMotorAction>>(60000, UINT32_MAX, 3000, 500,
                                                                                                                 ApplicationDevices::getInstance().getVibratingMotor())),
                          _neoPixel(ApplicationDevices::getInstance().getNeoPixel()),
                          _gpsReceiver(ApplicationDevices::getInstance().getGpsReceiver())

    {
    }

    // Destructor is still needed to explicitly stop the periodic action before
    // its unique_ptr goes out of scope and deletes the object.
    ~GPSReceiverAction()
    {
        if (_periodicNeopixelAction)
            _periodicNeopixelAction->stop();

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    // Prevent copying and assignment, as unique_ptr is not copyable
    GPSReceiverAction(const GPSReceiverAction &) = delete;
    GPSReceiverAction &operator=(const GPSReceiverAction &) = delete;

    // Allow moving if necessary (optional, but good practice)
    GPSReceiverAction(GPSReceiverAction &&) = default;
    GPSReceiverAction &operator=(GPSReceiverAction &&) = default;

    void performAction();
    bool isActive();

private:
    SpeciesZone::Zone _previousZone = SpeciesZone::Zone::UNKNOWN_ZONE;

    NeoPixel &_neoPixel;
    std::unique_ptr<PeriodicAction<NeoPixelAction>> _periodicNeopixelAction;

    SoundPlayer &_soundPlayer;
    std::unique_ptr<SoundButtonAction> _soundButtonAction_frog;
    std::unique_ptr<SoundButtonAction> _soundButtonAction_pigeon;

    std::unique_ptr<PeriodicAction<VibratingMotorAction>> _periodicVibratingMotorAction_long;
    std::unique_ptr<PeriodicAction<VibratingMotorAction>> _periodicVibratingMotorAction_short;

    std::unique_ptr<PeriodicAction<MotorAction>> _periodicMotorAction_frog_long;
    std::unique_ptr<PeriodicAction<MotorAction>> _periodicMotorAction_frog_short;
    std::unique_ptr<PeriodicAction<MotorAction>> _periodicMotorAction_pigeon_long;
    std::unique_ptr<PeriodicAction<MotorAction>> _periodicMotorAction_pigeon_short;

    GPSReceiver &_gpsReceiver;
    std::atomic<bool> _isActive{false};

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
    void setActive(bool value);
};
