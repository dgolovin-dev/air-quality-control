#ifndef RELAYMODEL_DEFINITION
#define RELAYMODEL_DEFINITION

enum RelayStates {
  OFF, ON, MAINTENANCE_OFF, MAINTENANCE_ON
};

class RelayModule {

  private:
    byte highPin; // sets HIGH, if the relay is open
    byte lowPin; // sets LOW, if the relay is open 

    bool* triggerState;
    RelayStates* relayState;
    unsigned long triggerVerifyDelay;

    unsigned long switchOnDuration;
    unsigned long switchOffDuration;
    unsigned long maintenancePeriod; // periodically switch in order to prevent stuck
    
    bool initialized = false;
    bool prevTriggerState = false;
    unsigned long triggerSwitchTime = 0;
    unsigned long stateSwitchTime = 0;

    bool update() {
      if(!this->initialized) {
        if(this->highPin > 0) {
          pinMode(this->highPin, OUTPUT);
        }
        if(this->highPin > 0) {
          pinMode(this->lowPin, OUTPUT);
        }
        this->initialized = true;
      }
      unsigned long now = millis();
      bool trigger = *(this->triggerState);
      if(trigger != this->prevTriggerState) {
        this->prevTriggerState = trigger;
        this->triggerSwitchTime = now;
      }
      bool triggerVerified = now - this->triggerSwitchTime > this->triggerVerifyDelay;
      
      bool isOn = *(this->relayState) == RelayStates::ON || *(this->relayState) == RelayStates::MAINTENANCE_ON;

      if(this->highPin > 0) {
        digitalWrite(this->highPin, isOn ? HIGH : LOW);
      } 
      if(this->lowPin > 0) {
        digitalWrite(this->lowPin, isOn ? LOW : HIGH);
      }

      unsigned long switchDelay = (now - this->stateSwitchTime);
      bool canSwitch = switchDelay > (isOn ? this->switchOnDuration : this->switchOffDuration);     
      
      if(canSwitch) {
        bool needMaintenance = this->maintenancePeriod != -1 && switchDelay > this->maintenancePeriod;
        if(isOn && !trigger && triggerVerified) {
          *(this->relayState) = RelayStates::OFF; 
          this->stateSwitchTime = now;
        } else if(!isOn && trigger && triggerVerified) {
          *(this->relayState) = RelayStates::ON; 
          this->stateSwitchTime = now;
        } else if (needMaintenance) {
          if(isOn) {
            *(this->relayState) = RelayStates::MAINTENANCE_OFF; 
          } else {
            *(this->relayState) = RelayStates::MAINTENANCE_ON; 
          }
          this->stateSwitchTime = now;
        }
      }

      return true;
    }

  public: 
    template <size_t max_tasks>
    RelayModule(
      bool* triggerField, RelayStates* stateField,
      unsigned long triggerVerifyDelay, unsigned long switchOnDuration, unsigned long switchOffDuration, unsigned long maintenancePeriod, 
      byte highPin, byte lowPin,
      unsigned long updatePeriod, UniTimer<max_tasks>* timer
    ) {
      this->triggerState = triggerField;
      this->relayState = stateField;
      this->triggerVerifyDelay = triggerVerifyDelay;
      this->switchOnDuration = switchOnDuration;
      this->switchOffDuration = switchOffDuration;
      this->maintenancePeriod = maintenancePeriod;
      this->highPin = highPin;
      this->lowPin = lowPin;
      timer->every(updatePeriod, &this->update, this);
    }
};


#endif
