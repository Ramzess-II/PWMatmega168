#include "eeprom.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)

{
	cli();
	while(EECR & (1<<EEPE)){} //ждем освобождения флага окончания последней операцией с памятью
	EEAR = uiAddress; //Устанавливаем адрес
	EEDR = ucData; //Пищем данные в регистр
	EECR |= (1<<EEMPE); //Разрешаем запись
	EECR |= (1<<EEPE); //Пишем байт в память
	sei();
}

uint8_t EEPROM_read(uint8_t uiAddress)

{
	cli();
	while(EECR & (1<<EEPE)){} //ждем освобождения флага окончания последней операцией с памятью
	EEAR = uiAddress; //Устанавливаем адрес
	EECR |= (1<<EERE); //Запускаем операцию считывания из памяти в регистр данных
	sei();
	return EEDR; //Возвращаем результат
	
}
