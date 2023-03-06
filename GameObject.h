#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
  
#include <Arduino.h>

class GameObject {
  private:
  public:
    GameObject* children[10];
    virtual GameObject();
    virtual void step(int delta);
//    virtual void onUpdate(Event* event);
};

#endif
