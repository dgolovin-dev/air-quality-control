#include "HardwareSerial.h"
#ifndef DEFINITION_UNITITMER
#define DEFINITION_UNITITMER


class UniTimer {
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

    UniTimer(ScheduledTask* tasks, unsigned int tasksCount) {
      this->tasks = tasks;
      this->tasksCount = tasksCount;
    }

    template<class T = void*, typename H = void (T::*)()>
    int every(unsigned long interval, H h, T* ctx){
      return this->every(interval, (void (*)(void*)) h, (void*)ctx);
    }

    int every(unsigned long interval, void h()){
      return this->every(interval, this->functionAdapter, h);
    }

    int every(unsigned long interval, Handler handler, void* ctx) {
      return schedule(interval, true, handler, ctx);
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

    tick(){
      auto now = millis();
      auto t = this->tasks;
      for(unsigned int i = 0; i < this->tasksCount; i++, t++) {
        if(!t->active) continue; 
        if(now - t->start < t->period) continue;
        t->handler(t->ctx);
        if(t->repeatable) {
          t->start = millis();
        } else {
          t-> active = false;
        }
      }
    }

  private:
    static void functionAdapter(void h()) {
      h();
    }

    ScheduledTask* tasks;
    unsigned int tasksCount;
};   

#endif