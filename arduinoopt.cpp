// https://blog.kriegsman.org/2013/12/01/optimizing-10-lines-of-arduino-code-youre-wrong-so-am-i/

uint8_t brightness_s( const int8_t led, const int8_t pos)
{
   int8_t bright[] = {1, 2, 6, 16, 32, 16, 6, 2, 1 };
   int8_t adj = 4 + led - pos;
   return bright[(~(adj >> 7)) & (-(adj - 8) >> 7) & adj];
}


uint8_t brightness_13(const int8_t led, const int8_t pos) { 
   int8_t diff; 
   diff = led - pos;
   if( diff < 0 ) diff = -diff;

   // test for the most common case first
   if( diff > 3 )  return 1;
   // then handle all the more special cases
   if( diff == 3)  return 2;
   if( diff == 2)  return 6;
   if( diff == 1)  return 16;
   /* diff == 0 */ return 32;
}
