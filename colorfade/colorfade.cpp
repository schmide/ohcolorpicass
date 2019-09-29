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
color onColor = { 255 * SCALOR, 20 * SCALOR , 0 * SCALOR },
offColor = { 0 * SCALOR, 0 * SCALOR, 255 * SCALOR },
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
   deltaColor.r = colorSpan.r / rate;
   deltaColor.g = colorSpan.g / rate;
   deltaColor.b = colorSpan.b / rate;
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
      currentColor.r = (currentColor.r + deltaColor.r);
      currentColor.g = (currentColor.g + deltaColor.g);
      currentColor.b = (currentColor.b + deltaColor.b);
   }
   // integers round down every cycle and dot products are not kind to integers.
   currentColor.r = currentColor.r < 0 ? 0 : currentColor.r;
   currentColor.g = currentColor.g < 0 ? 0 : currentColor.g;
   currentColor.b = currentColor.b < 0 ? 0 : currentColor.b;
   color outputColor;
   outputColor.r = currentColor.r / SCALOR;
   outputColor.g = currentColor.g / SCALOR;
   outputColor.b = currentColor.b / SCALOR;
   flux = flux;

}

int main()
{
   while (true) {
      UpdateColor();
   }
}

