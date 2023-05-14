#include "./scheduler.h"

class HumidifierTriggerModule {
  public:
    struct Args {
      float* inHumidity;
      float lowHumidity;
      bool* outState;
      Scheduler* timer;
      unsigned long updatePeriod;
    };
    HumidifierTriggerModule(Args args) {
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }
  private:
    Args args;
    void update(){
      *(args.outState) = *(args.inHumidity) < args.lowHumidity;
    }
};