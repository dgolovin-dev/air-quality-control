#include <EEPROM.h>

class Mq135CalibrationModule {
  private:
    float* currentRZeroState;
    float* calibratedRZeroState;
    bool* btnState;
    unsigned long persistDelay;
    unsigned int persistAddr;
    bool prevBtnState;

    bool update() {
      if(this->prevBtnState == false && *(this->btnState) == true) {
        // button pushed
        if(millis() > this->persistDelay) {
          EEPROM.put(this->persistAddr, *(this->currentRZeroState));
          Serial.println("calibrated");
        } else {
          Serial.println("calibration skipped");
        }
      }
      this->prevBtnState = *(this->btnState);
      EEPROM.get(this->persistAddr, *(this->calibratedRZeroState));
      return true;
    }
  public:
    template <size_t max_tasks>
    Mq135CalibrationModule(
      float* currentRZeroField, float* calibratedRZeroField, bool* btnField,
      unsigned long persistDelay, unsigned int persistCell, 
      unsigned long updatePeriod, UniTimer<max_tasks>* timer
    ){
      this->currentRZeroState = currentRZeroField;
      this->calibratedRZeroState = calibratedRZeroField;
      this->btnState = btnField;
      this->persistDelay = persistDelay;
      this->persistAddr = persistAddr;
      timer->every(updatePeriod, &this->update, this);
    }
};