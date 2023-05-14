#include "./MQ135.h"

class Mq135Module {
  public:
    struct Args {
      float* inHumidity;
      float* inTemperature;
      float* inRZero;
      float rload;
      float* outRZero;
      float* outPpm;
      uint8_t pin;
      UniTimer* timer;
      unsigned long updatePeriod;
    };

    Mq135Module(Args args) {
      this->args = args;
      args.timer->every(args.updatePeriod, &this->update, this);
    }
  private:
    Args args;
    
    bool update() {
      auto mq135 = MQ135(args.pin, *(args.inRZero), args.rload);
      auto h = *(args.inHumidity);
      auto t = *(args.inTemperature);
      *(args.outRZero) = mq135.getCorrectedRZero(t, h);
      *(args.outPpm) = mq135.getCorrectedPPM(t, h);
      return true;
    } 
  
};