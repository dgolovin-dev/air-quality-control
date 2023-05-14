#include "./uniTimer.h"

class HeatTriggerModule {
  public:
    struct Args {
      float* inCurrentTemperature;
      float* inLowTemperature;
      bool* outTrigger;
      UniTimer* timer;
      unsigned long updatePeriod;
    };

    HeatTriggerModule(Args args) {
      this->args = args;
      args.timer->every(args.updatePeriod, &this->update, this);
    }

  private:
    Args args;

    void update(){
      auto t = *(args.inCurrentTemperature);
      auto l = *(args.inLowTemperature);
      *(args.outTrigger) = t < l;
    }
 };