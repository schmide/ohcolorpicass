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
#define FLOOR 1

struct color {
   COLORTYPE r, g, b;
};

color onColor = { 255,255,255 }, 
      offColor = { 0,0,0 }, 
      currentColor = offColor;
STEPTYPE steps = 32;
bool flux = true;

void CheckUnderflow(COLORTYPE &delta, COLORTYPE &oldDelta, COLORTYPE &current, COLORTYPE &dest)
{
   if (abs(delta) < FLOOR) {// underflow compensation 
      if (oldDelta < 0) {
         delta = -FLOOR;
      } else {
         if (oldDelta > 0) {
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
   STEPTYPE stepsDirection;
   color dest;
   if (flux) {
      dest = onColor;
      stepsDirection = steps;
   } else {
      dest = offColor;
      stepsDirection = -steps;
   }
   color deltaColor;
   deltaColor.r = colorSpan.r / stepsDirection;
   deltaColor.g = colorSpan.g / stepsDirection;
   deltaColor.b = colorSpan.b / stepsDirection;
   CheckUnderflow(deltaColor.r, colorSpan.r, currentColor.r, dest.r);
   CheckUnderflow(deltaColor.b, colorSpan.b, currentColor.b, dest.b);
   CheckUnderflow(deltaColor.g, colorSpan.g, currentColor.g, dest.g);
   DOTTYPE vectDot = (dest.r - currentColor.r) * deltaColor.r + (dest.g - currentColor.g) * deltaColor.g + (dest.b - currentColor.b) * deltaColor.b;
   if ( vectDot <= 0 ) {
      currentColor = dest;
//      flux = !flux; // no flux given
   } else {
      currentColor.r += deltaColor.r;
      currentColor.g += deltaColor.g;
      currentColor.b += deltaColor.b;
   }
}

int main()
{
   while (true) {
      UpdateColor();
   }
}

