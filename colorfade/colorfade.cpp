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
#define USESHIFT
#ifdef USESHIFT
#define FLOOR 1
#define SCALOR 7
#define RATE 5
#else
#define FLOOR 1
#define SCALOR 128
#define RATE 32
#endif

struct color {
   COLORTYPE r, g, b;
};

STEPTYPE scalor = SCALOR;
STEPTYPE rate = RATE;
#ifdef USESHIFT
#define FIXCOLOR(a) ((a) << scalor)
#define UNFIXCOLOR(a) ((a) >> scalor)
#define SETRATE(a) ((a) >> rate)
#else
#define FIXCOLOR(a) ((a) * scalor)
#define UNFIXCOLOR(a) ((a) / scalor)
#define SETRATE(a) ((a) / rate)
#endif
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
   color dest;
   if (flux) {
      dest = onColor;
      colorSpan.r = onColor.r - offColor.r;
      colorSpan.g = onColor.g - offColor.g;
      colorSpan.b = onColor.b - offColor.b;
   } else {
      dest = offColor;
      colorSpan.r = offColor.r - onColor.r;
      colorSpan.g = offColor.g - onColor.g;
      colorSpan.b = offColor.b - onColor.b;
   }
   color deltaColor;
   deltaColor.r = SETRATE(colorSpan.r);
   deltaColor.g = SETRATE(colorSpan.g);
   deltaColor.b = SETRATE(colorSpan.b);
   CheckUnderflow(deltaColor.r, colorSpan.r, currentColor.r, dest.r);
   CheckUnderflow(deltaColor.b, colorSpan.b, currentColor.b, dest.b);
   CheckUnderflow(deltaColor.g, colorSpan.g, currentColor.g, dest.g);
   DOTTYPE vectDot = (dest.r - currentColor.r) * deltaColor.r;
   vectDot += (dest.g - currentColor.g) * deltaColor.g;
   vectDot += (dest.b - currentColor.b) * deltaColor.b;
   if ( vectDot <= 0 ) {
      currentColor = dest;
//      flux = !flux; // no flux given
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
//   flux = flux;
}

int main()
{
   while (true) {
      UpdateColor();
   }
}

