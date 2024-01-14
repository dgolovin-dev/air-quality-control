#include "RBDdimmer.h";

class RbdDimmerComponent {
   public:
    struct Args {
      bool* inTrigger;
      unsigned long triggerVerifyPeriod;
      unsigned long switchPeriod;
      unsigned long maintenancePeriod;
      unsigned long adjustPeriod;

      uint8_t lowLevel;
      uint8_t highLevel;
      uint8_t* level;
      uint8_t dimPin;

      unsigned long* triggerTime;
      unsigned long* switchTime;
      unsigned long* adjustTime;
     
      Scheduler* timer;
      unsigned long updatePeriod;
    };

    RbdDimmerComponent(Args args):dimmer(args.dimPin) {
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }
  private:
    Args args;
    
    bool initialized = false;
    dimmerLamp dimmer;

    bool prevTriggerState;
    unsigned long triggerTime;
    unsigned long switchTime;
    unsigned long adjustTime;

    bool update() {
      unsigned long now = millis();
      if(!this->initialized) {       
        this->initialized = true;
        this->prevTriggerState = false;
        this->triggerTime = now-this->args.triggerVerifyPeriod-1;
        this->switchTime = now-this->args.switchPeriod-1;
        this->adjustTime = now-this->args.adjustPeriod-1;
        this->dimmer.begin(NORMAL_MODE, OFF);
        Serial.println();
      }
     
      bool trigger = *(args.inTrigger);
      if(trigger != this->prevTriggerState) {
        this->prevTriggerState = trigger;
        this->triggerTime = now;
      }
      bool triggerVerified = now - this->triggerTime > args.triggerVerifyPeriod;    

      uint8_t level = *(this->args.level);
      uint8_t prevLevel = level;

      unsigned long switchDelay = now - this->switchTime;
      unsigned long adjustDelay = now - this->adjustTime;
      unsigned long triggerDelay = now - this->triggerTime;

      if(level < this->args.lowLevel && switchDelay > this->args.maintenancePeriod) {
          level = this->args.lowLevel;
          this->switchTime = now;
          this->adjustTime = now;
          this->triggerTime = now;
      }else if(triggerVerified) {
        if(trigger && level < this->args.lowLevel && switchDelay > this->args.switchPeriod) { // switch on
          level = this->args.lowLevel;
         this->switchTime = now;
          this->adjustTime = now;
          this->triggerTime = now;
        } else if(trigger && level >= this->args.lowLevel && level < this->args.highLevel && adjustDelay > this->args.adjustPeriod) { // increase power
          level++;
          this->adjustTime = now;
          this->triggerTime = now;
        } else if(!trigger && level > this->args.lowLevel && adjustDelay > this->args.adjustPeriod) {
          level--;       
          this->adjustTime = now;
          this->triggerTime = now;
        } else if(!trigger && level == this->args.lowLevel && adjustDelay > this->args.adjustPeriod && switchDelay > this->args.switchPeriod) {
          level = 0;
         this->switchTime = now;
          this->adjustTime = now;
          this->triggerTime = now;
        }
      }

      if(this->dimmer.getPower() != level) {
        this->dimmer.setPower(level);      
      }
      if(this->dimmer.getState() != (level >= this->args.lowLevel)) {
        this->dimmer.changeState();
      }     
      
      *(this->args.level) = level;
      *(this->args.triggerTime) = triggerDelay;
      *(this->args.switchTime) = switchDelay;
      *(this->args.adjustTime) = adjustDelay;

      return true;
    } 
};
