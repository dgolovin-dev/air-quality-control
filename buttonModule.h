#include "./uniTimer.h"

class ButtonModule {
  private:
    typedef void (*Handler)(void*,bool);

    static void functionAdapter(void h(bool), bool v) {
      h(v);
    }

    bool initialized;
    byte pin;
    bool* state;
    
    bool update() {
      if(!this->initialized) {
        pinMode(this->pin, INPUT);
        //Serial.print("init button pin ");
        //Serial.println(this->pin);
        this->initialized = true;
      } else {
        auto signal = digitalRead(this->pin);
   
        bool state = *(this->state);
        if(signal == HIGH) {
          state = true;
        }
        if(signal == LOW) {
          state = false;
        }
        *(this->state) = state;
      }
      return true;
    }   
  public:
    template <size_t max_tasks>
    ButtonModule(bool* field, byte pin, unsigned long checkPeriod, UniTimer<max_tasks>* timer){
      this->initialized = false;
      this->state = field;
      this->pin = pin;
      timer->every(checkPeriod, &this->update, this);
    }
};