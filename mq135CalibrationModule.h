#include <EEPROM.h>

class Mq135CalibrationModule {
  public:
    struct Args {
      float* inRZero;
      bool* inBtn;
      unsigned long calibrationDelay;
      float* outRZero;      
      unsigned int persistAddr;
      Scheduler* timer;
      unsigned long updatePeriod;
    };

    Mq135CalibrationModule(Args args){
      this->args = args;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
    }

  private:
    Args args;
    bool prevBtnState;

    void update() {
      if(this->prevBtnState == false && *(args.inBtn) == true) {
        // button pushed
        if(millis() > args.calibrationDelay) {
          EEPROM.put(args.persistAddr, *(args.inRZero));
          Serial.println("calibrated");
        } else {
          Serial.println("calibration skipped");
        }
      }
      this->prevBtnState = *(args.inBtn);
      EEPROM.get(args.persistAddr, *(args.outRZero));
    }
 
};