#pragma cplusplus on

extern inline float sqrtf(float x)
{
static const double _half=.5;
static const double _three=3.0;
volatile float y;
 if(x > 0.0f)
 {


   double guess = __frsqrte((double)x);   // returns an approximation to   
   guess = _half*guess*(_three - guess*guess*x);  // now have 12 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 24 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits
   y=(float)(x*guess);
   return y ;
 }
  return x ;
}   

extern inline float sqrt(float x)
{
static const double _half=.5;
static const double _three=3.0;
volatile float y;
 if(x > 0.0f)
 {


   double guess = __frsqrte((double)x);   // returns an approximation to   
   guess = _half*guess*(_three - guess*guess*x);  // now have 12 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 24 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits
   guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits

   y=(float)(x*guess);
   return y ;
 }
  return x ;
}

extern inline float fabs(float x)
            {
#if __MIPS__
			 return fabsf(x);
#else
             (*(int*)&x)&=0x7fffffff;
             return  x;
#endif
            }

#pragma cplusplus reset
