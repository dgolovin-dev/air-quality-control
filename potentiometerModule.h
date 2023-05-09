#include "./uniTimer.h"

class PotentiometerModule {
  private:
    bool initialized;
    byte pin;
    bool reversed;
    float* state;
    float minValue;
    float maxValue;

    bool update() {
      auto signal = analogRead(this->pin);
      if(reversed) {
        signal = 1023 - signal;
      }
      *(this->state) = this->minValue + (this->maxValue - this->minValue) * (float)signal/1023.0;      
      return true;
    }

  public: 
    template <size_t max_tasks>
    PotentiometerModule(float* field, byte pin, bool reversed, float minValue, float maxValue, unsigned long updatePeriod, UniTimer<max_tasks>* timer) {
      this->pin = pin;
      this->reversed = reversed;
      this->state = field;
      this->minValue = minValue;
      this->maxValue = maxValue;
      timer->every(updatePeriod, &this->update, this);
    }
};