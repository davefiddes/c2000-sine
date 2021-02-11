/*
 * This file is part of the tumanako_vc project.
 *
 * Copyright (C) 2011 Johannes Huebner <dev@johanneshuebner.com>
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

#include "my_string.h"
#ifdef STM32F1
#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#endif
#include "terminal.h"
#include "hwdefs.h"
#include "device.h"
#include <stdarg.h>

static const TERM_CMD *CmdLookup(char *buf);
static void term_send(const char *str);

extern const TERM_CMD TermCmds[];
static char inBuf[TERM_BUFSIZE];

void term_Init()
{
}

/** Run the terminal */
void term_Run()
{
   char args[TERM_BUFSIZE];
   const TERM_CMD *pCurCmd = NULL;
   int currentIdx = 0;

   while (1)
   {
      while ( SCI_getRxFIFOStatus( TERM_USART ) != SCI_FIFO_RX0 )
      {
          uint16_t rxStatus = SCI_getRxStatus(TERM_USART);
          if((rxStatus & SCI_RXSTATUS_ERROR) != 0)
          {
              // Just die here until we figure out how to reset the UART
              ESTOP0;
          }

         char newChar = SCI_readCharNonBlocking( TERM_USART );
         SCI_writeCharBlockingFIFO(TERM_USART, newChar);
         inBuf[currentIdx++] = newChar;
      }

      if (currentIdx > 0)
      {
         if (inBuf[currentIdx - 1] == '\n' || inBuf[currentIdx - 1] == '\r')
         {
            term_send("\r\n");
            inBuf[currentIdx] = 0;
            char *space = (char*)my_strchr(inBuf, ' ');

            if (0 == *space) //No args after command, look for end of line
            {
               space = (char*)my_strchr(inBuf, '\n');
               args[0] = 0;
            }
            else //There are arguments, copy everything behind the space
            {
               my_strcpy(args, space + 1);
            }

            if (0 == *space) //No \n found? try \r
               space = (char*)my_strchr(inBuf, '\r');

            *space = 0;
            pCurCmd = CmdLookup(inBuf);

            if (NULL != pCurCmd)
            {
                pCurCmd->CmdFunc(args);
            }
            else if (currentIdx > 1)
            {
               term_send("Unknown command sequence\r\n");
            }
            currentIdx = 0;
         }
         else if (inBuf[0] == '!' && NULL != pCurCmd)
         {
            currentIdx = 0;
            pCurCmd->CmdFunc(args);
         }
      }
   } /* while(1) */
} /* term_Run */

// Needed for printf
extern "C" int putchar(int c)
{
    SCI_writeCharBlockingFIFO(TERM_USART, c);
    return c;
}

static const TERM_CMD *CmdLookup(char *buf)
{
   const TERM_CMD *pCmd = TermCmds;

   for (; NULL != pCmd->cmd; pCmd++)
   {
      if (0 == my_strcmp(buf, pCmd->cmd))
      {
         break;
      }
   }
   if (NULL == pCmd->cmd)
   {
      pCmd = NULL;
   }
   return pCmd;
}

static void term_send(const char *str)
{
    for (;*str > 0; str++)
        SCI_writeCharBlockingFIFO(TERM_USART, *str);
}


