#include "./uniTimer.h"

class HumidifierTriggerModule {
  private:
    bool* triggerState;
    float* humidityState;
    float lowHumidity;

    bool update(){
      *(this->triggerState) = *(this->humidityState) < this->lowHumidity;
      return true;
    }

  public:
    template <size_t max_tasks>
    HumidifierTriggerModule(bool* triggerField, float* humidityField, float lowHumidity, unsigned long updatePeriod, UniTimer<max_tasks>* timer) {
      this->triggerState = triggerField;
      this->humidityState = humidityField;
      this->lowHumidity = lowHumidity;
      timer->every(updatePeriod, &this->update, this);
    }
};