// colorfade.cpp : Just to Bother
//

#include <iostream>
#include "colorfade.h"

STEPTYPE scalor = SCALOR;
STEPTYPE rate = RATE;
color onColor = { FIXCOLOR(255), FIXCOLOR(20), FIXCOLOR(0) };
color offColor = { FIXCOLOR(0) , FIXCOLOR(0), FIXCOLOR(255) };
color currentColor = offColor;
color deltaColor;
bool dirty = true;

void SetColors(color &newOff, color &newOn)
{
   offColor.r = FIXCOLOR(newOff.r);
   offColor.g = FIXCOLOR(newOff.g);
   offColor.b = FIXCOLOR(newOff.b);
   onColor.r = FIXCOLOR(newOn.r);
   onColor.g = FIXCOLOR(newOn.g);
   onColor.b = FIXCOLOR(newOn.b);
   currentColor = offColor;
   dirty = true;
}

void GetCurrentColor(color &current)
{
   current.r = UNFIXCOLOR(currentColor.r);
   current.g = UNFIXCOLOR(currentColor.g);
   current.b = UNFIXCOLOR(currentColor.b);
}

void SetScaleRate(STEPTYPE newScalor, STEPTYPE newRate)
{
   scalor = newScalor;
   rate = newRate;
   currentColor = offColor;
   dirty = true;
}

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

bool UpdateColor(bool on)
{
   if (dirty) {
      color colorSpan;
      colorSpan.r = onColor.r - offColor.r;
      colorSpan.g = onColor.g - offColor.g;
      colorSpan.b = onColor.b - offColor.b;
      deltaColor.r = SETRATE(colorSpan.r);
      deltaColor.g = SETRATE(colorSpan.g);
      deltaColor.b = SETRATE(colorSpan.b);
      CheckUnderflow(deltaColor.r, colorSpan.r, currentColor.r, offColor.r);
      CheckUnderflow(deltaColor.b, colorSpan.b, currentColor.b, offColor.b);
      CheckUnderflow(deltaColor.g, colorSpan.g, currentColor.g, offColor.g);
      dirty = false;
   } 
   color deltaColorDirection;
   color dest;
   if (on) {
      dest = onColor;
      deltaColorDirection = deltaColor;
   } else {
      dest = offColor;
      deltaColorDirection.r = -deltaColor.r;
      deltaColorDirection.g = -deltaColor.g;
      deltaColorDirection.b = -deltaColor.b;
   }
   DOTTYPE vectDot = (dest.r - currentColor.r) * deltaColorDirection.r;
   vectDot += (dest.g - currentColor.g) * deltaColorDirection.g;
   vectDot += (dest.b - currentColor.b) * deltaColorDirection.b;
   if ( vectDot <= 0 ) {
      currentColor = dest;
      return false;
   } else {
      currentColor.r += deltaColorDirection.r;
      currentColor.g += deltaColorDirection.g;
      currentColor.b += deltaColorDirection.b;
   }
#ifdef SECONDFLOOR
   // integers round down every cycle and dot products are not kind to integers.
   currentColor.r = currentColor.r < 0 ? 0 : currentColor.r;
   currentColor.g = currentColor.g < 0 ? 0 : currentColor.g;
   currentColor.b = currentColor.b < 0 ? 0 : currentColor.b;
#endif
   return true;
}
