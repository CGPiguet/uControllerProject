#ifndef flashSave_H
#define flashSave_H

#include "main.h"
void Write_Flash(uint8_t data)
{
     HAL_FLASH_Unlock();
     __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
     FLASH_Erase_Sector(FLASH_SECTOR_6, VOLTAGE_RANGE_3);
     HAL_FLASH_Program(TYPEPROGRAM_WORD, FlashAddress, data);
     HAL_FLASH_Lock();
}


#endif // JOYSTICK_H

