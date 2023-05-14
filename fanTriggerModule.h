#include "./scheduler.h"

class FanTriggerModule {
  public:
    struct Args {
      float* inPpm;
      float normalPpm;
      float highPpm;
      bool* outValue;
      Scheduler* timer;
      unsigned long updatePeriod;
    };

    FanTriggerModule(Args args) {
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }
  private:
    Args args;

    bool update(){
      if(*(args.outValue)){
        if(*(args.inPpm) < args.normalPpm) {
          *(args.outValue) = false;
        }
      } else {
        if(*(args.inPpm) > args.highPpm) {
          *(args.outValue) = true;
        }
      }            
      return true;
    }


};