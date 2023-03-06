#ifndef TRIANGLE_H
#define TRIANGLE_H
 
#include <Arduino.h>
#include "Adafruit_ILI9341.h"
#include "GameObject.h"

class Triangle: GameObject {
  private:
    Adafruit_ILI9341* tft;
    int x;
    int y;
    int h;
    int w;
    int last;
  public:
    GameObject* children[10];
    Triangle(Adafruit_ILI9341*, int, int, int, int);
    void step(int delta);
//    override void onUpdate(Event* event);
};

#endif
