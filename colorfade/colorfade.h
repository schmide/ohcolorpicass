#ifndef _COLORFADE
#define _COLORFADE_EXTERN 
#endif

// Color and step should be at least 16 bits 
// dot product can overflow but overflow causes capture.
// floor can be very low for floats
#define COLORTYPE short int  
#define STEPTYPE short int  
#define DOTTYPE long int  
#define USESHIFT
//#define SECONDFLOOR
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

#ifdef USESHIFT
#define FIXCOLOR(a) ((a) << scalor)
#define UNFIXCOLOR(a) ((a) >> scalor)
#define SETRATE(a) ((a) >> rate)
#else
#define FIXCOLOR(a) ((a) * scalor)
#define UNFIXCOLOR(a) ((a) / scalor)
#define SETRATE(a) ((a) / rate)
#endif

// moves color to on or off. 
// Returns false if zero movement (i.e. at endpoints)
_COLORFADE_EXTERN bool UpdateColor(bool on = true);
_COLORFADE_EXTERN void SetColors(color &newOff, color &newOn);
_COLORFADE_EXTERN void GetCurrentColor(color &current);
_COLORFADE_EXTERN void SetScaleRate(STEPTYPE newScalor, STEPTYPE newRate);

#ifndef _COLORFADE
#define _COLORFADE
#undef _COLORFADE_EXTERN
#define _COLORFADE_EXTERN extern
#endif

