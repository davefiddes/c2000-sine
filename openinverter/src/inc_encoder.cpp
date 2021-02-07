/*
 * This file is part of the stm32-sine project.
 *
 * Copyright (C) 2011 Johannes Huebner <dev@johanneshuebner.com>
 * Copyright (C) 2019 Nail G�zel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "hwdefs.h"
#include "inc_encoder.h"
#include "my_math.h"
#include "errormessage.h"
#include "params.h"
#include "sine_core.h"
#include "printf.h"

#define TWO_PI            65536
//Angle difference at which we assume jitter to become irrelevant
#define STABLE_ANGLE      ((10 * TWO_PI) / 360)
#define MAX_CNT           TWO_PI - 1
#define MAX_REVCNT_VALUES 5
#define MIN_RES_AMP       1000

#define FRQ_TO_PSC(frq) ((72000000 / frq) - 1)
#define NUM_ENCODER_CONFIGS (sizeof(encoderConfigurations) / sizeof(encoderConfigurations[0]))


void Encoder::Reset()
{
}

/** Since power up, have we seen the north marker? */
bool Encoder::SeenNorthSignal()
{
   return false;
}

/** Set type of motor position feedback
 * @param mode type of motor position feedback */
void Encoder::SetMode(Encoder::mode mode)
{
}

void Encoder::SetPwmFrequency(uint32_t frq)
{
}

/** set number of impulses per shaft rotation
  */
void Encoder::SetImpulsesPerTurn(uint16_t imp)
{
}

void Encoder::SwapSinCos(bool swap)
{
}

void Encoder::UpdateRotorAngle(int dir)
{
}

/** Update rotor frequency.
 * @param callingFrequency Frequency at which this function is called in Hz
 */
void Encoder::UpdateRotorFrequency(int callingFrequency)
{
}

/** Returns current angle of motor shaft to some arbitrary 0-axis
 * @return angle in digit (2Pi=65536)
*/
uint16_t Encoder::GetRotorAngle()
{
   return 0;
}

/** Return rotor frequency in Hz
 * @pre in AB/ABZ encoder mode UpdateRotorFrequency must be called at a regular interval */
u32fp Encoder::GetRotorFrequency()
{
   return 0;
}

int Encoder::GetRotorDirection()
{
   return 0;
}

/** Get current speed in rpm */
uint32_t Encoder::GetSpeed()
{
    return 0;
}

/** Get number of rotor turns since power up */
uint32_t Encoder::GetFullTurns()
{
   return 0;
}

