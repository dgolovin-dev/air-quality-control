#include "./uniTimer.h"

class HeatTriggerModule {
  private:
    bool* triggerState;
    float* temperatureState;
    float* lowTemperatureState;

    bool update(){
      auto t = *(this->temperatureState);
      auto l = *(this->lowTemperatureState);
      *(this->triggerState) = t < l;
      return true;
    }
  public:
    template <size_t max_tasks>
    HeatTriggerModule(bool* triggerField, float* temperatureField, float* lowTemperatureField, unsigned long updatePeriod, UniTimer<max_tasks>* timer) {
      this->triggerState = triggerField;
      this->temperatureState = temperatureField;
      this->lowTemperatureState = lowTemperatureField;
      timer->every(updatePeriod, &this->update, this);
    }
};