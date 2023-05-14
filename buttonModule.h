#include "./scheduler.h"

class ButtonModule {  
  public:
    struct Args {
      bool* outValue;
      uint8_t pin;
      Scheduler* timer;
      unsigned int updatePeriod;      
    };

    ButtonModule(Args args){
      this->initialized = false;
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }
    
  private:
    Args args;
    bool initialized;    

    bool update() {
      if(!this->initialized) {
        pinMode(this->args.pin, INPUT);
        //Serial.print("init button pin ");
        //Serial.println(this->pin);
        this->initialized = true;
      } else {
        auto signal = digitalRead(this->args.pin);
   
        bool state = *(this->args.outValue);
        if(signal == HIGH) {
          state = true;
        }
        if(signal == LOW) {
          state = false;
        }
        *(this->args.outValue) = state;
      }
      return true;
    }   
};