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

#ifdef STM32F1
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/crc.h>
#endif
#include "params.h"
#include "param_save.h"
#include "hwdefs.h"
#include "my_string.h"

#define NUM_PARAMS ((PARAM_BLKSIZE - 8) / sizeof(PARAM_ENTRY))
#define PARAM_WORDS (PARAM_BLKSIZE / 4)

typedef struct
{
   uint16_t key;
   uint8_t dummy;
   uint8_t flags;
   uint32_t value;
} PARAM_ENTRY;

typedef struct
{
   PARAM_ENTRY data[NUM_PARAMS];
   uint32_t crc;
   uint32_t padding;
} PARAM_PAGE;

/**
* Save parameters to flash
*
* @return CRC of parameter flash page
*/
uint32_t parm_save()
{
#if STM32F1
    PARAM_PAGE parmPage;
   unsigned int idx;

   crc_reset();
   memset32((int*)&parmPage, 0xFFFFFFFF, PARAM_WORDS);

   //Copy parameter values and keys to block structure
   for (idx = 0; Param::IsParam((Param::PARAM_NUM)idx) && idx < NUM_PARAMS; idx++)
   {
      const Param::Attributes *pAtr = Param::GetAttrib((Param::PARAM_NUM)idx);
      parmPage.data[idx].flags = (uint8_t)Param::GetFlag((Param::PARAM_NUM)idx);
      parmPage.data[idx].key = pAtr->id;
      parmPage.data[idx].value = Param::Get((Param::PARAM_NUM)idx);
   }

   parmPage.crc = crc_calculate_block(((uint32_t*)&parmPage), (2 * NUM_PARAMS));
   flash_unlock();
   flash_erase_page(PARAM_ADDRESS);

   for (idx = 0; idx < PARAM_WORDS; idx++)
   {
      uint32_t* pData = ((uint32_t*)&parmPage) + idx;
      flash_program_word(PARAM_ADDRESS + idx * sizeof(uint32_t), *pData);
   }
   flash_lock();
   return parmPage.crc;
#else
   return 0x4242;
#endif
}

/**
* Load parameters from flash
*
* @retval 0 Parameters loaded successfully
* @retval -1 CRC error, parameters not loaded
*/
int parm_load()
{
#if STM32F1
    PARAM_PAGE *parmPage = (PARAM_PAGE *)PARAM_ADDRESS;

   crc_reset();
   uint32_t crc = crc_calculate_block(((uint32_t*)parmPage), (2 * NUM_PARAMS));

   if (crc == parmPage->crc)
   {
      for (unsigned int idxPage = 0; idxPage < NUM_PARAMS; idxPage++)
      {
         Param::PARAM_NUM idx = Param::NumFromId(parmPage->data[idxPage].key);
         if (idx != Param::PARAM_INVALID && parmPage->data[idxPage].key > 0)
         {
            Param::SetFlt(idx, parmPage->data[idxPage].value);
            Param::SetFlagsRaw(idx, parmPage->data[idxPage].flags);
         }
      }
      return 0;
   }
#endif

   return -1;
}
