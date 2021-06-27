
#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
uint8_t EEPROM_read(uint8_t uiAddress);




#endif /* EEPROM_H_ */
