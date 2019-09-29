#include "pch.h"
#include <iostream>
#include "colorfade.h"

int main()
{
   bool flux = true;
   while (true) {
      if(!UpdateColor(flux))
         flux = !flux; // no flux given
      color outputColor;
      GetCurrentColor(outputColor);
      flux = flux;
   }
}

