#ifndef flashSave_H
#define flashSave_H

#include "main.h"
//__attribute__((__section__(".user_data"))) const uint8_t userConfig[64];
void Write_Flash(uint8_t data)
{
     HAL_FLASH_Unlock();
     __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
     FLASH_Erase_Sector(FLASH_SECTOR_6, VOLTAGE_RANGE_3);
     HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[0], data);
     HAL_FLASH_Lock();


//     HAL_FLASH_Unlock();
//     __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
//     FLASH_Erase_Sector(FLASH_SECTOR_, VOLTAGE_RANGE_);
//     HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[index], someData);
//     HAL_FLASH_Lock();
}


#endif // JOYSTICK_H

