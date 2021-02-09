#ifndef INC_ENCODER_H_INCLUDED
#define INC_ENCODER_H_INCLUDED

#include <stdint.h>
#include "my_fp.h"

class Encoder
{
public:
   enum mode
   {
      SINGLE, AB, ABZ, SPI, RESOLVER, SINCOS, INVALID
   };

   static void Reset();
   static void SetMode(enum mode encMode);
   static bool SeenNorthSignal();
   static void UpdateRotorAngle(int dir);
   static void UpdateRotorFrequency(int callingFrequency);
   static void SetPwmFrequency(uint32_t frq);
   static uint16_t GetRotorAngle();
   static uint32_t GetSpeed();
   static uint32_t GetFullTurns();
   static u32fp GetRotorFrequency();
   static int GetRotorDirection();
   static void SetImpulsesPerTurn(uint16_t imp);
   static void SwapSinCos(bool swap);

private:
};

#endif // INC_ENCODER_H_INCLUDED
