// colorfade.cpp : Just to Bother
//

#include "pch.h"
#include <iostream>

// Color and step should be at least 16 bits 
// dot product can overflow but overflow causes capture.
// sigma can be very low for floats
#define COLORTYPE short int  
#define STEPTYPE short int  
#define DOTTYPE long int  
#define SIGMA 1

struct color {
   COLORTYPE r, g, b;
};

color onColor = { 255,255,255 }, offColor = { 0,0,0 }, currentColor = offColor;
STEPTYPE steps = 32;
bool flux = true;

void CheckUnderflow(COLORTYPE &delta, COLORTYPE &oldDelta, COLORTYPE &current, COLORTYPE &dest)
{
   if (delta * delta < SIGMA) // underflow compensation 
      if (oldDelta < 0)
         delta = -SIGMA;
      else
         if (oldDelta > 0)
            delta = SIGMA;
         else
            current = dest;
}

void UpdateColor()
{
   color deltaColor = onColor;
   deltaColor.r -= offColor.r;
   deltaColor.g -= offColor.g;
   deltaColor.b -= offColor.b;
   STEPTYPE stepsDirection;
   color dest;
   if (flux) {
      dest = onColor;
      stepsDirection = steps;
   } else {
      dest = offColor;
      stepsDirection = -steps;
   }
   color oldDeltaColor = deltaColor;
   deltaColor.r /= stepsDirection;
   deltaColor.g /= stepsDirection;
   deltaColor.b /= stepsDirection;
   CheckUnderflow(deltaColor.r, oldDeltaColor.r , currentColor.r, deltaColor.r);
   CheckUnderflow(deltaColor.b, oldDeltaColor.b , currentColor.b, deltaColor.b);
   CheckUnderflow(deltaColor.g, oldDeltaColor.g , currentColor.g, deltaColor.g);
   DOTTYPE vectDot = (dest.r - currentColor.r) * deltaColor.r; 
   vectDot += (dest.g - currentColor.g) * deltaColor.g;
   vectDot += (dest.b - currentColor.b) * deltaColor.b;
   if ( vectDot <= 0 ) {
      currentColor = dest;
      flux = !flux; // no flux given
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

/*
class color {
public:
   color &operator+=(color otherColor) { r += otherColor.r; g += otherColor.g; b += otherColor.b; return *this;  }
   color &operator-=(color otherColor) { r -= otherColor.r; g -= otherColor.g; b -= otherColor.b; return *this;  }
   color &operator*=(int mult ) { r *= mult; g *= mult; b *= mult; return *this;  }
   color &operator/=(int divisor ) { r /= divisor; g /= divisor; b /= divisor; return *this;  }
   int r, g, b;
};

color onColor = { 255,0,0 }, offColor = { 0,255,0 }, currentColor = onColor;

int steps = 4;

bool flux = false;

void UpdateColor()
{
   color deltaColor = onColor;
   deltaColor -= offColor;
   deltaColor /= flux ? steps : -steps;
   color dest = flux ? onColor : offColor;
   if (abs(currentColor.r - dest.r) < abs(deltaColor.r) || abs(currentColor.g - dest.g) < abs(deltaColor.g) || abs(currentColor.b - dest.b) < abs(deltaColor.b) ) {
      currentColor = dest;
   } else {
      currentColor += deltaColor;
   }
}


int main()
{
   while (true) {
      UpdateColor();
   }
    std::cout << "Hello World!\n"; 
}

*/
