#include "DHT.h"
#include "./scheduler.h"

class DhtModule {
  public:
    struct Args {
      float* outHumidity;
      float* outTemperature;
      uint8_t dhtType;
      uint8_t pin;
      Scheduler* timer;
      unsigned long updatePeriod;      
    };

    DhtModule(Args args)
    : dht(args.pin, args.dhtType) {
      this->args = args;
      this->initialized = false;
      args.timer->schedule(args.updatePeriod, true, &this->update, this);
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