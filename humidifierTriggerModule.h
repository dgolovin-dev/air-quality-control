#include "./uniTimer.h"

class HumidifierTriggerModule {
  public:
    struct Args {
      float* inHumidity;
      float lowHumidity;
      bool* outState;
      UniTimer* timer;
      unsigned long updatePeriod;
    };
    HumidifierTriggerModule(Args args) {
      this->args = args;
      args.timer->every(args.updatePeriod, &this->update, this);
    }
  private:
    Args args;
    void update(){
      *(args.outState) = *(args.inHumidity) < args.lowHumidity;
    }
};