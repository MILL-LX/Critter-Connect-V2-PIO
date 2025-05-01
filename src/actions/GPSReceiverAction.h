#include <atomic>

#include "devices/GPSReceiver.h"
#include "devices/NeoPixel.h"
#include "devices/SoundPlayer.h"

#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"

#include "app/SpeciesZone.h"

class GPSReceiverAction
{
public:
    GPSReceiverAction()
    {
        _neopixel = new NeoPixel();
        _periodicNeopixelAction = new PeriodicAction<NeoPixelAction>(1000, 5, 500, NeoPixel::StateColor::OK);
        _soundPlayer = new SoundPlayer();
    }

    ~GPSReceiverAction()
    {
        delete _neopixel;
        _neopixel = nullptr;

        _periodicNeopixelAction->stop();
        delete _periodicNeopixelAction;
        _periodicNeopixelAction = nullptr;

        delete _soundPlayer;
        _soundPlayer = nullptr;
    }

    void performAction();

private:
    std::atomic<bool> _is_running{false};
    SpeciesZone::Zone _previousZone = SpeciesZone::Zone::NON_SPECIES_ZONE;

    NeoPixel *_neopixel;
    PeriodicAction<NeoPixelAction> *_periodicNeopixelAction;
    SoundPlayer *_soundPlayer;

    void processLocationUpdate(GPSReceiver::GPSData gpsData);
};