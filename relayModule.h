#ifndef RELAYMODEL_DEFINITION
#define RELAYMODEL_DEFINITION

enum RelayStates {
  OFF, ON, MAINTENANCE_OFF, MAINTENANCE_ON
};

class RelayModule {
   public:
    struct Args {
      bool* inTrigger;
      unsigned long triggerVerifyPeriod;
      unsigned long switchOnDuration;
      unsigned long switchOffDuration;
      unsigned long maintenancePeriod;
      RelayStates* outState;
      uint8_t highPin;
      uint8_t lowPin;
      Scheduler* timer;
      unsigned long updatePeriod;
    };

    RelayModule(Args args) {
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }
  private:
    Args args;

    bool initialized = false;
    bool prevTriggerState = false;
    unsigned long triggerSwitchTime = 0;
    unsigned long stateSwitchTime = 0;

    void update() {
      if(!this->initialized) {
        if(args.highPin > 0) {
          pinMode(args.highPin, OUTPUT);
        }
        if(args.lowPin > 0) {
          pinMode(args.lowPin, OUTPUT);
        }
        this->initialized = true;
      }
      unsigned long now = millis();
      bool trigger = *(args.inTrigger);
      if(trigger != this->prevTriggerState) {
        this->prevTriggerState = trigger;
        this->triggerSwitchTime = now;
      }
      bool triggerVerified = now - this->triggerSwitchTime > args.triggerVerifyPeriod;
      
      bool isOn = *(args.outState) == RelayStates::ON || *(args.outState) == RelayStates::MAINTENANCE_ON;

      if(args.highPin > 0) {
        digitalWrite(args.highPin, isOn ? HIGH : LOW);
      } 
      if(args.lowPin > 0) {
        digitalWrite(args.lowPin, isOn ? LOW : HIGH);
      }

      unsigned long switchDelay = (now - this->stateSwitchTime);
      bool canSwitch = switchDelay > (isOn ? args.switchOnDuration : args.switchOffDuration);     
      
      if(canSwitch) {
        bool needMaintenance = args.maintenancePeriod > 0 && switchDelay > args.maintenancePeriod;
        if(isOn && !trigger && triggerVerified) {
          *(args.outState) = RelayStates::OFF; 
          this->stateSwitchTime = now;
        } else if(!isOn && trigger && triggerVerified) {
          *(args.outState) = RelayStates::ON; 
          this->stateSwitchTime = now;
        } else if (needMaintenance) {
          if(isOn) {
            *(args.outState) = RelayStates::MAINTENANCE_OFF; 
          } else {
            *(args.outState) = RelayStates::MAINTENANCE_ON; 
          }
          this->stateSwitchTime = now;
        }
      }
    } 
};
#endif
