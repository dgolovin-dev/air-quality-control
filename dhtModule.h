#include "DHT.h"
#include "./uniTimer.h"

class DhtModule {
  private:
    bool initialized;
    float* humidityState;
    float* temperatureState;
    DHT dht;

    bool update() {
      if(!this->initialized) {
        this->initialized = true;
        this->dht.begin();
      }
      *(this->humidityState) = this->dht.readHumidity();
      *(this->temperatureState) = this->dht.readTemperature();
      return true;
    } 
  public:
    template <size_t max_tasks>
    DhtModule(float* humidityField, float* temperatureField, byte pin, byte dhtType, unsigned long updatePeriod, UniTimer<max_tasks>* timer)
    : dht(pin, dhtType) {
      this->initialized = false;
      this->humidityState = humidityField;
      this->temperatureState = temperatureField;
      timer->every(updatePeriod, &this->update, this);
    }
};