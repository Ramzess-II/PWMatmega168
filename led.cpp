#include "led.h"

void indikciya (char symbol){  // сюда попадают символы для отображения
	
	switch (symbol)
	{
		case 'H' :
		PORTD  |= 0x01;        // сначала выключаем все ножки которые идут на 7 сегментники 
		PORTC  |= 0x3F;
		PORTC  &= ~0x36;       // потом включаем те что нам нужны
		PORTD  &= ~0x01;		
		break;
		case 'S' :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x1B;
		PORTD  &= ~0x01;
		break;
	}
	
}

void indikciya (int symbol){    // перегрузка функции сюда попадают числа
	
	switch (symbol)
	{
		case 0 :
		PORTD  |= 0x01;          // все тоже самое
		PORTC  |= 0x3F;
		PORTC  &= ~0x3F;
		break;
		case 1 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x30;
		break;
		case 2 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x2D;
		PORTD  &= ~0x01;
		break;
		case 3 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x39;
		PORTD  &= ~0x01;
		break;
		case 4 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x32;
		PORTD  &= ~0x01;
		break;
		case 5 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x1B;
		PORTD  &= ~0x01;
		break;
		case 6 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x1F;
		PORTD  &= ~0x01;
		break;
		case 7 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x31;
		break;
		case 8 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x3F;
		PORTD  &= ~0x01;
		break;
		case 9 :
		PORTD  |= 0x01;
		PORTC  |= 0x3F;
		PORTC  &= ~0x3B;
		PORTD  &= ~0x01;
		break;
	}
	
}
