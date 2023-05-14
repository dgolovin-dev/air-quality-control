#include "./scheduler.h"

class HeatTriggerModule {
  public:
    struct Args {
      float* inCurrentTemperature;
      float* inLowTemperature;
      bool* outTrigger;
      Scheduler* timer;
      unsigned long updatePeriod;
    };

    HeatTriggerModule(Args args) {
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }

  private:
    Args args;

    void update(){
      auto t = *(args.inCurrentTemperature);
      auto l = *(args.inLowTemperature);
      *(args.outTrigger) = t < l;
    }
 };