#include "./scheduler.h"
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

const int taskCount = 12;
Scheduler::ScheduledTask tasks[taskCount];
auto timer = Scheduler(tasks, taskCount); 

auto registry = Registry();

auto button = ButtonModule({  
  .outValue = &registry.calBtn, 
  .pin = 12, 
  .timer = &timer,
  .updatePeriod = 100
});

auto potentiometer = PotentiometerModule({
  .reversed = true, 
  .minValue = 10.0, 
  .maxValue = 30.0,
  .outValue = &registry.targetTemperature, 
  .pin = A0,
  .timer = &timer,
  .updatePeriod = 100
});

auto dhtModule = DhtModule({
  .outHumidity = &registry.humidity,
  .outTemperature = &registry.temperature,
  .dhtType = DHT11,
  .pin = 2,
  .timer = &timer,
  .updatePeriod = 100  
});


auto mq135Module = Mq135Module({
  .inHumidity = &registry.humidity, 
  .inTemperature = &registry.temperature,
  .inRZero = &registry.calibratedRZero,
  .rload = 10,
  .outRZero = &registry.currentRZero, 
  .outPpm = &registry.ppm, 
  .pin = A1, 
  .timer = &timer,
  .updatePeriod = 100
});
auto mq135CalibrationModule = Mq135CalibrationModule({
  .inRZero = &registry.currentRZero,
  .inBtn = &registry.calBtn,
  .calibrationDelay = 1000L,
  .outRZero =  &registry.calibratedRZero,
  .persistAddr = 0,
  .timer = &timer,
  .updatePeriod = 100
});


// humidity dev +/- 3
// rload 1k
// cal rzero = 30
// 1w later rzero = 16 ???
// - replaced sensor rload 10k
// fri rzero 16 (first)
// sat rzero 44/46/47
// sun rzero 49/52/50/57 (rain) // humidity correction wrong?
// mon 52/49/47/43 (clear)
// tue 43
// wed 45/33
// thu 47/46
// mon 45
auto fanTrigger = FanTriggerModule({
  .inPpm = &registry.ppm,
  .normalPpm = 600,
  .highPpm = 800,
  .outValue = &registry.fanTrigger,
  .timer = &timer,
  .updatePeriod = 100
});
auto fanRelay = RelayModule({
  .inTrigger = &registry.fanTrigger,
  .triggerVerifyPeriod = 10000,
  .switchOnDuration = 60000,
  .switchOffDuration = 60000,
  .maintenancePeriod = 60L*60*1000,
  .outState = &registry.fanRelay,
  .highPin = 8,
  .lowPin = 3,
  .timer = &timer,
  .updatePeriod = 100
});

auto heatTrigger = HeatTriggerModule({
   .inCurrentTemperature = &registry.temperature,
   .inLowTemperature = &registry.targetTemperature,
   .outTrigger = &registry.heatTrigger,
   .timer = &timer,
   .updatePeriod = 100
});
auto heatRelay = RelayModule({
  .inTrigger = &registry.heatTrigger,
  .triggerVerifyPeriod = 10000,
  .switchOnDuration = 120000,
  .switchOffDuration = 120000,
  .maintenancePeriod = 60L*60*1000,
  .outState = &registry.heatRelay,
  .highPin = 9,
  .lowPin = 4,
  .timer = &timer,
  .updatePeriod = 100
});

auto humidifierTrigger = HumidifierTriggerModule({
    .inHumidity = &registry.humidity,
    .lowHumidity = 40,
    .outState = &registry.humidifierTrigger,
    .timer = &timer,
    .updatePeriod = 100
});
auto humidifierRelay = RelayModule({
  .inTrigger = &registry.humidifierTrigger,
  .triggerVerifyPeriod = 10000,
  .switchOnDuration = 120000,
  .switchOffDuration = 120000,
  .maintenancePeriod = 60L*60*1000,
  .outState = &registry.heatRelay,
  .highPin = 10,
  .lowPin = 5,
  .timer = &timer,
  .updatePeriod = 100
});

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();  
  lcd.backlight();
  lcd.clear();
  
  Serial.begin(9600);
  Serial.println("setup");
  timer.schedule(1000, true, &printState);
}

void printState() {
  registry.printToSerial();
  registry.printToDisplay16x2(&lcd);
}

void loop() {  
  timer.tick();
}
