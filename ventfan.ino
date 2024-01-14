#include "./scheduler.h"
#include "./registry.h"
#include "./dhtComponent.h"
#include "./mhz19Component.h"
#include "./fanTriggerComponent.h"
#include "./rbdDimmerComponent.h"

const int taskCount = 5;
Scheduler::ScheduledTask tasks[taskCount];
auto timer = Scheduler(tasks, taskCount); 

auto registry = Registry();

auto dht22Component = Dht22Component({
  .outHumidity = &registry.humidity,
  .outTemperature = &registry.temperature,
  .pin = 12,
  .timer = &timer,
});

auto mhz19Component = Mhz19Component({
  .outPpmValue = &registry.ppm,
  .pinRx = 9,
  .pinTx = 8,
  .timer = &timer
});

auto fanTriggerComponent = FanTriggerComponent({
  .inPpm = &registry.ppm,
  .inHumidity = &registry.humidity,
  .highPpm = 800,
  .highHumididty = 60,
  .outValue = &registry.fanTrigger,
  .timer = &timer,
  .updatePeriod = 6*1000L
});

auto rbdDimmerComponent = RbdDimmerComponent({
  .inTrigger = &registry.fanTrigger,
  .triggerVerifyPeriod = 18*1000L,
  .switchPeriod = 6*60*1000L,
  .maintenancePeriod = 54*60*1000L,
  .adjustPeriod = 18*1000L,
  .lowLevel = 40,
  .highLevel = 70,
  .level = &registry.fanLevel,
  .dimPin = 4,
  .triggerTime = &registry.fanTriggerTime,
  .switchTime = &registry.fanSwitchTime,
  .adjustTime = &registry.fanAdjustTime,
  .timer = &timer,
  .updatePeriod = 6*1000L
});

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("start"); 
  timer.schedule(6*1000, true, &registry.printToSerial, &registry);
}

void loop() {  
  timer.tick();
}
