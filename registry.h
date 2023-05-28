#import "./relayModule.h"
#import "./LiquidCrystal_I2C.h"

struct Registry {
  bool calBtn = false;
  float targetTemperature = 0;

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

  void printToSerial();
  void printToDisplay16x2(LiquidCrystal_I2C* lcd);
};


void Registry::printToSerial() {
    Serial.print("workTime=");
    Serial.print(millis());

    Serial.print("\t");
    Serial.print("calBtn=");
    Serial.print(this->calBtn);

    Serial.print("\t");
    Serial.print("targetTemperature=");
    Serial.print(this->targetTemperature);

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

  void Registry::printToDisplay16x2(LiquidCrystal_I2C* lcd) {       
    lcd->setCursor(0,0);
    lcd->print("t");
    lcd->print(this->temperature,1);
    lcd->print(" h");
    lcd->print(this->humidity,0);
    lcd->print(" c");
    lcd->print(this->ppm, 0);
    lcd->print("  "); // CO2 length may differ

    lcd->setCursor(0,1);
    lcd->print("tt");
    lcd->print(this->targetTemperature,1);
    lcd->print(" u");
    lcd->print(this->humidifierTrigger);
    lcd->print(this->humidifierRelay);
    lcd->print("e");
    lcd->print(this->heatTrigger);
    lcd->print(this->heatRelay);
    lcd->print("a");
    lcd->print(this->fanTrigger);
    lcd->print(this->fanRelay);
  }