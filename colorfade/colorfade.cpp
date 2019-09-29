// colorfade.cpp : Just to Bother
//

#include "pch.h"
#include <iostream>

// Color and step should be at least 16 bits 
// dot product can overflow but overflow causes capture.
// floor can be very low for floats
#define COLORTYPE short int  
#define STEPTYPE short int  
#define DOTTYPE long int  
#define FLOOR 4
#define SCALOR 128
#define RATE 32

struct color {
   COLORTYPE r, g, b;
};

STEPTYPE scalor = SCALOR;
STEPTYPE rate = RATE;
#define FIXCOLOR(a) ((a) * scalor)
#define UNFIXCOLOR(a) ((a) / scalor)
#define SETRATE(a) ((a) / rate)
color onColor = { FIXCOLOR(255), FIXCOLOR(20), FIXCOLOR(0) },
offColor = { FIXCOLOR(0) , FIXCOLOR(0), FIXCOLOR(255) },
currentColor = offColor;
bool flux = true;

void CheckUnderflow(COLORTYPE &delta, COLORTYPE &span, COLORTYPE &current, COLORTYPE &dest)
{
   if (abs(delta) < FLOOR) {// underflow compensation 
      if (span < 0) {
         delta = -FLOOR;
      } else {
         if (span > 0) {
            delta = FLOOR;
         } else {
            current = dest;
         }
      }
   }
}

void UpdateColor()
{
   color colorSpan;
   colorSpan.r = onColor.r - offColor.r;
   colorSpan.g = onColor.g - offColor.g;
   colorSpan.b = onColor.b - offColor.b;
   color dest;
   color deltaColor;
   deltaColor.r = SETRATE(colorSpan.r);
   deltaColor.g = SETRATE(colorSpan.g);
   deltaColor.b = SETRATE(colorSpan.b);
   if (flux) {
      dest = onColor;
   } else {
      dest = offColor;
      deltaColor.r = -deltaColor.r ;
      deltaColor.g = -deltaColor.g ;
      deltaColor.b = -deltaColor.b ;
   }
   CheckUnderflow(deltaColor.r, colorSpan.r, currentColor.r, dest.r);
   CheckUnderflow(deltaColor.b, colorSpan.b, currentColor.b, dest.b);
   CheckUnderflow(deltaColor.g, colorSpan.g, currentColor.g, dest.g);
   DOTTYPE vectDot = (dest.r - currentColor.r) * deltaColor.r;
   vectDot += (dest.g - currentColor.g) * deltaColor.g;
   vectDot += (dest.b - currentColor.b) * deltaColor.b;
   if ( vectDot <= 0 ) {
      currentColor.r = dest.r;
      currentColor.g = dest.g;
      currentColor.b = dest.b;
      flux = !flux; // no flux given
   } else {
      currentColor.r = currentColor.r + deltaColor.r;
      currentColor.g = currentColor.g + deltaColor.g;
      currentColor.b = currentColor.b + deltaColor.b;
   }
   // integers round down every cycle and dot products are not kind to integers.
   currentColor.r = currentColor.r < 0 ? 0 : currentColor.r;
   currentColor.g = currentColor.g < 0 ? 0 : currentColor.g;
   currentColor.b = currentColor.b < 0 ? 0 : currentColor.b;
   color outputColor;
   outputColor.r = UNFIXCOLOR(currentColor.r);
   outputColor.g = UNFIXCOLOR(currentColor.g);
   outputColor.b = UNFIXCOLOR(currentColor.b);
   flux = flux;
}

int main()
{
   while (true) {
      UpdateColor();
   }
}

