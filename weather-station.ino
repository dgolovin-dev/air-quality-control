#include "./uniTimer.h"
#include "./registry.h"
#include "./buttonModule.h"
#include "./potentiometerModule.h"
#include "./dhtModule.h"
#include "./mq135Module.h"
#include "./mq135CalibrationModule.h"
#include "./relayModule.h"
#include "./fanTriggerModule.h"
#include "./heatTriggerModule.h"
#include "./humidifierTriggerModule.h"

auto timer = UniTimer<12>(); 
auto registry = Registry();

auto button = ButtonModule(&registry.btn, 12, 100, &timer);
auto potentiometer = PotentiometerModule(&registry.potentiometer, A0, true, 10, 30, 100, &timer);

auto dhtModule = DhtModule(&registry.humidity, &registry.temperature, 2, DHT11, 100, &timer);

auto mq135Module = Mq135Module(&registry.humidity, &registry.temperature, &registry.calibratedRZero, &registry.currentRZero, &registry.ppm, A1, 1, 100, &timer);
auto mq135CalibrationModule = Mq135CalibrationModule(&registry.currentRZero, &registry.calibratedRZero, &registry.btn, 24L*60*60*1000, 0, 100, &timer);

auto fanTrigger = FanTriggerModule(&registry.fanTrigger, &registry.ppm, 600, 800, 100, &timer);
auto fanRelay = RelayModule(&registry.fanTrigger, &registry.fanRelay, 10000, 60000, 60000, 60L*60*1000, 8, 3, 100, &timer);

auto heatTrigger = HeatTriggerModule(&registry.heatTrigger, &registry.temperature, &registry.potentiometer, 100, &timer);
auto heatRelay = RelayModule(&registry.heatTrigger, &registry.heatRelay, 10000, 120000, 120000, 60L*60*1000, 8, 4, 100, &timer);

auto humidifierTrigger = HumidifierTriggerModule(&registry.humidifierTrigger, &registry.humidity, 40, 100, &timer);
auto humidifierRelay = RelayModule(&registry.humidifierTrigger, &registry.humidifierRelay, 10000, 120000, 120000, -1, 9, 5, 100, &timer);

void setup() {  
  Serial.begin(9600);
  Serial.println("setup");
  timer.every(5000, &registry.printFull, &registry);
}

void loop() {  
  timer.tick();
}
