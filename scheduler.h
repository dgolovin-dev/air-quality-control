#include "HardwareSerial.h"
//#include "rbdDimmerModule.h"
#ifndef DEFINITION_SCHEDULLER
#define DEFINITION_SCHEDULLER 

#define MIN_DELAY 500

//#define LOG_EXEC_TIME

class Scheduler {
  public: 
    typedef void (*Handler)(void* ctx);
    struct ScheduledTask {
      bool active = false;
      Handler handler;
      void* ctx;
      unsigned long start;
      unsigned long period;
      unsigned long repeatable;      
    };
    
    Scheduler(ScheduledTask* tasks, unsigned int tasksCount) {
      this->tasks = tasks;
      this->tasksCount = tasksCount;
    }

    template<class T = void*, typename H = void (T::*)()>
    int schedule(unsigned long interval, bool repeatable, H h, T* ctx){
      return this->schedule(interval, repeatable, (void (*)(void*)) h, (void*)ctx);
    }

    int schedule(unsigned long interval, bool repeatable, void h()){
      return this->schedule(interval, repeatable, this->functionAdapter, h);
    }

    int schedule(unsigned long period, bool repeatable, Handler handler, void* ctx) {
      auto t = this->tasks;
       for(int i = 0; i < this->tasksCount; i++, t++) {
        if(t->active) continue; 
        t->active = true;
        t->handler = handler;
        t->ctx = ctx;
        t->start = millis;
        t->period = period;
        t->repeatable = repeatable;
        return i; 
      }
      Serial.println("can't schedule a task");
      return -1;
    }

    int tickTaskIdx = 0;
    uint32_t lastExecTime;

    tick(){
      auto now = millis();
      if(now - lastExecTime < MIN_DELAY) return;

      if(this->tasksCount == 0) {
        return;
      }

      if(tickTaskIdx >= this->tasksCount) {
        tickTaskIdx = 0;
      }      

      auto t = this->tasks + tickTaskIdx;
      tickTaskIdx++;

      if(!t->active) return; 
      if(now - t->start < t->period) return;

#ifdef LOG_EXEC_TIME
unsigned long start = micros();
#endif
        
      t->handler(t->ctx);
  
#ifdef LOG_EXEC_TIME
unsigned long end = micros();
unsigned long delta = end - start;
Serial.print(tickTaskIdx-1);
Serial.print(" task exec time ");
Serial.println(delta);
#endif

      if(t->repeatable) {
        t->start = millis();
      } else {
        t-> active = false;
      }

      lastExecTime = now;
    }

  private:
    static void functionAdapter(void h()) {
      h();
    }

    ScheduledTask* tasks;
    unsigned int tasksCount;
};
#endif