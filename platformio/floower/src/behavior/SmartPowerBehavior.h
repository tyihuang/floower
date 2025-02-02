#pragma once

#include "Arduino.h"
#include "Config.h"
#include "hardware/Floower.h"
#include "BluetoothControl.h"
#include "behavior/Behavior.h"

#define STATE_STANDBY 0
#define STATE_OFF 1
#define STATE_LOW_BATTERY 2
#define STATE_BLUETOOTH_PAIRING 3
#define STATE_REMOTE_CONTROL 4
// states 128+ are reserved for child behaviors

class SmartPowerBehavior : public Behavior {
    public:
        SmartPowerBehavior(Config *config, Floower *floower, BluetoothControl *remote);
        virtual void setup(bool wokeUp = false);
        virtual void loop();
        virtual bool isIdle();
        
    protected:
        virtual bool onLeafTouch(FloowerTouchEvent event);
        virtual bool onRemoteChange(StateChangePacketData data);
        virtual bool canInitializeBluetooth();

        void changeStateIfIdle(state_t fromState, state_t toState);
        void changeState(uint8_t newState);
        HsbColor nextRandomColor();

        Config *config;
        Floower *floower;
        BluetoothControl *bluetoothControl;

        uint8_t state;
        bool preventTouchUp = false;

    private:
        void enablePeripherals(bool initial, bool wokeUp);
        void disablePeripherals();        
        void powerWatchDog(bool initial = false, bool wokeUp = false);
        void planDeepSleep(long timeoutMs);
        void enterDeepSleep();
        void indicateStatus(uint8_t status, bool enable);

        PowerState powerState;
        unsigned long colorsUsed = 0; // used by nextRandomColor

        unsigned long watchDogsTime = 0;
        unsigned long bluetoothStartTime = 0;
        unsigned long deepSleepTime = 0;

        uint8_t indicatingStatus = 0;
    
};