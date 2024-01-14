#include "./scheduler.h"

class FanTriggerComponent {
  public:
    struct Args {
      uint16_t* inPpm;
      float* inHumidity;
      
      uint16_t highPpm;     
      float highHumididty;

      bool* outValue;
      Scheduler* timer;
      unsigned long updatePeriod;
    };

    FanTriggerComponent(Args args) {
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }
  private:
    Args args;

    bool update(){
      *(args.outValue) = *(args.inPpm) > args.highPpm || *(args.inHumidity) > args.highHumididty;             
      return true;
    }


};