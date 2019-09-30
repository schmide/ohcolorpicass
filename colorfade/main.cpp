#include <iostream>
#include "colorfade.h"

int main()
{
   bool flux = true;
   color onColor = { 255, 0,0 };
   color offColor = { 0 , 0, 255 };
   SetColors(offColor, onColor);
   while (true) {
      if(!UpdateColor(flux))
         flux = !flux; // no flux given
      color outputColor;
      GetCurrentColor(outputColor);
      flux = flux;
   }
}

