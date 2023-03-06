#include "Triangle.h"
    
Triangle::Triangle(Adafruit_ILI9341* tft, int x, int y, int h, int w) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->h = h;
  this->w = w;
  this->last = 0;
}
    
void Triangle::step(int delta) {
   last += delta;
   last = last % 10000;
   tft->fillTriangle(x, y, x+w, y, x+w, y+h, 0xFFFF);//(last / 322) << 11);
}
      
//void Triangle::onUpdate(Event* event) {
//
//}
