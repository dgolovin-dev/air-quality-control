#include "./scheduler.h"
#include "./MHZ19.h"
#include "AltSoftSerial.h"
#include <SoftwareSerial.h>     

#define MHZ19_WARMUP_TIME 180000
#define MHZ19_READ_INTERVAL 5100 // 5000 by docs
#define MHZ19_CALIBRATION_POINT_PERIOD 4*60*1000L
#define MHZ19_CALIBRATION_POINTS 5// approx 20 min
#define MHZ19_LOW_LEVEL 400

class Mhz19Component {
  public: 
    struct Args {
      uint16_t* outPpmValue;
      uint8_t pinRx;
      uint8_t pinTx;
      Scheduler* timer;
    };

    Mhz19Component(Args args): serial(args.pinRx, args.pinTx, false), mhz19()  {
      this->args = args;
      args.timer->schedule(MHZ19_READ_INTERVAL, true, &this->update, this);
    }

  private:
    Args args;
    bool initialized; 
    MHZ19 mhz19;
    AltSoftSerial serial;
    uint16_t calibrationPoints[MHZ19_CALIBRATION_POINTS];
    uint8_t calibrationPointsIdx;
    uint32_t calibrationPointTime;

    bool update() {
      if(!this->initialized) {
        this->serial.begin(9600);
        this->mhz19.begin(this->serial);
        this->mhz19.autoCalibration(); 

        this->initialized = true; 
        // fill calibration array
        for(uint8_t i = 0; i < MHZ19_CALIBRATION_POINTS; i ++) {
          this->calibrationPoints[i] = MHZ19_LOW_LEVEL;
        }
        this->calibrationPointsIdx = 0;
      }

      int val = this->mhz19.getCO2();
      if(this->mhz19.errorCode == RESULT_FILTER) {
        return;
      }

      *(this->args.outPpmValue) = val;
      
      // autolcalibrate low level
      if(millis() - this->calibrationPointTime > MHZ19_CALIBRATION_POINT_PERIOD){
        this->calibrationPointTime = millis();
        this->calibrationPoints[this->calibrationPointsIdx] = val;
        this->calibrationPointsIdx = (this->calibrationPointsIdx + 1) % MHZ19_CALIBRATION_POINTS;
        bool allCalValsLower = true;
        for(uint8_t i = 0; i < MHZ19_CALIBRATION_POINTS; i ++) {
          allCalValsLower = allCalValsLower && this->calibrationPoints[i] < MHZ19_LOW_LEVEL;
        }
        if(allCalValsLower) {
          Serial.println("mhz19 calibrate");
          this->mhz19.calibrate();
          this->mhz19.autoCalibration(); 
          for(uint8_t i = 0; i < MHZ19_CALIBRATION_POINTS; i ++) {
            this->calibrationPoints[i] = MHZ19_LOW_LEVEL;
          }
        }
      }     
    }
};