#include "DHT22.h"
#include "./scheduler.h"

#define DHT_PERIOD 3000

class Dht22Component {
  public:
    struct Args {
      float* outHumidity;
      float* outTemperature;
      uint8_t pin;
      Scheduler* timer;   
    };

    Dht22Component(Args args)
    : dht(args.pin) {
      this->args = args;
      this->initialized = false;
      args.timer->schedule(DHT_PERIOD, true, &this->update, this);
    }

  private:
    Args args;
    DHT22 dht;
    bool initialized;


    bool update() {
      if(!this->initialized) {
        this->initialized = true;
      }
      auto err = dht.readData();
      if(err != 0) {
        //Serial.print("dht error: ");
        //Serial.println(err);
        return;
      }
      *(args.outHumidity) = this->dht.getHumidity();
      *(args.outTemperature) = this->dht.getTemperatureC();
      return true;
    } 

};