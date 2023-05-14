#import "./relayModule.h"

class Registry {
  public:
    bool btn = false;
    float potentiometer = 0;

    float humidity = 0;
    float temperature = 0;

    float calibratedRZero = 52;
    float currentRZero = 0;
    float ppm = 0;

    bool humidifierTrigger = false;
    RelayStates humidifierRelay = RelayStates::OFF;

    bool heatTrigger = false;
    RelayStates heatRelay = RelayStates::OFF;

    bool fanTrigger = false;
    RelayStates fanRelay = RelayStates::OFF;
    
    void printFull() const {
      Serial.print("workTime=");
      Serial.print(millis());

      Serial.print("\t");
      Serial.print("btn=");
      Serial.print(this->btn);

      Serial.print("\t");
      Serial.print("potentiometer=");
      Serial.print(this->potentiometer);

      Serial.print("\t");
      Serial.print("humidity=");
      Serial.print(this->humidity);

      Serial.print("\t");
      Serial.print("temperature=");
      Serial.print(this->temperature);

      Serial.print("\t");
      Serial.print("calibratedRZero=");
      Serial.print(this->calibratedRZero);

      Serial.print("\t");
      Serial.print("currentRZero=");
      Serial.print(this->currentRZero);

      Serial.print("\t");
      Serial.print("ppm=");
      Serial.print(this->ppm);

      Serial.print("\t");
      Serial.print("humidifierTrigger=");
      Serial.print(this->humidifierTrigger);

      Serial.print("\t");
      Serial.print("humidifierRelay=");
      Serial.print(this->humidifierRelay);

      Serial.print("\t");
      Serial.print("heatTrigger=");
      Serial.print(this->heatTrigger);

      Serial.print("\t");
      Serial.print("heatRelay=");
      Serial.print(this->heatRelay);

      Serial.print("\t");
      Serial.print("fanTrigger=");
      Serial.print(this->fanTrigger);

      Serial.print("\t");
      Serial.print("fanRelay=");
      Serial.print(this->fanRelay);

      Serial.println();
    }
};
