#include "DHT.h"
#include "./uniTimer.h"

class DhtModule {
  public:
    struct Args {
      float* outHumidity;
      float* outTemperature;
      uint8_t dhtType;
      uint8_t pin;
      UniTimer* timer;
      unsigned long updatePeriod;      
    };

    DhtModule(Args args)
    : dht(args.pin, args.dhtType) {
      this->args = args;
      this->initialized = false;
      args.timer->every(args.updatePeriod, &this->update, this);
    }

  private:
    Args args;
    DHT dht;
    bool initialized;


    bool update() {
      if(!this->initialized) {
        this->initialized = true;
        this->dht.begin();
      }
      *(args.outHumidity) = this->dht.readHumidity();
      *(args.outTemperature) = this->dht.readTemperature();
      return true;
    } 

};