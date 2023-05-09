#ifndef DEFINITION_UNITITMER
#define DEFINITION_UNITITMER

#include "./arduino-timer.h"

template<size_t max_tasks, class BaseTimer = Timer<max_tasks, millis, void*>>
class UniTimer : private BaseTimer {
  private:
  static bool functionAdapter(bool h()) {
    return h();
  }

  public: 
    template<class T = void*, typename H = bool (T::*)()>

    void* every(unsigned long interval, H h, T* opaque){
      return this->BaseTimer::every(interval, (bool (*)(void*)) h, (void*)opaque);
    }

    void* every(unsigned long interval, bool h()){
      return this->every(interval, this->functionAdapter, h);
    }

    tick(){
      this->BaseTimer::tick();
    }
};   

#endif