#include "./MQ135.h"

class Mq135Module {
  private:
    byte pin;
    float rload;

    float* humidityState;
    float* temperatureState;

    float* calibratedRZeroState;
    float* currentRZeroState;
    float* ppmState;
    
    bool update() {
      auto mq135 = MQ135(pin, (float) *this->calibratedRZeroState, rload);
      auto h = *(this->humidityState);
      auto t = *(this->temperatureState);
      *(this->currentRZeroState) = mq135.getCorrectedRZero(t, h);
      *(this->ppmState) = mq135.getCorrectedPPM(t, h);
      return true;
    } 
  public:
    template <size_t max_tasks>
    Mq135Module(
      float* humidityField, float* temperatureField, 
      float* calibratedRZeroField, float* currentRZeroField,
      float* ppmField,
      byte pin,
      float rload,
      unsigned long updatePeriod, UniTimer<max_tasks>* timer
    ) {
      this->humidityState = humidityField;
      this->temperatureState = temperatureField;
      this->calibratedRZeroState = calibratedRZeroField;
      this->currentRZeroState = currentRZeroField;
      this->ppmState = ppmField; 
      this->pin = pin;   
      this->rload = rload;  
      timer->every(updatePeriod, &this->update, this);
    }
};