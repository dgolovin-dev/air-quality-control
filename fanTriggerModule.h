#include "./uniTimer.h"

class FanTriggerModule {
  private:
    bool* triggerState;
    float* ppmState;

    float normalPpm;
    float highPpm;

    bool update(){
      bool isOn = *(this->triggerState);
      if(isOn){
        if(*(this->ppmState) < this->normalPpm) {
          *(this->triggerState) = false;
        }
      } else {
        if(*(this->ppmState) > this->highPpm) {
          *(this->triggerState) = true;
        }
      }            
      return true;
    }
  public:
    template <size_t max_tasks>
    FanTriggerModule(bool* triggerField, float* ppmField, float normalPpm, float highPpm, unsigned long updatePeriod, UniTimer<max_tasks>* timer) {
      this->triggerState = triggerField;
      this->ppmState = ppmField;
      this->normalPpm = normalPpm;
      this->highPpm = highPpm;
      timer->every(updatePeriod, &this->update, this);
    }

};