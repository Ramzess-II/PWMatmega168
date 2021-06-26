#include "main.h"

//------------------------------------------------------------------------------
// объявляем переменные
//------------------------------------------------------------------------------
volatile uint16_t impuls,dlitelnost;
volatile uint8_t simvol,razr1,razr2,razr3,knopkainkrement,otrabotat,pwm,menu,nastroika_chastoti;
volatile bool nazhatie_knop, yderzhanie_knop,pik,zvuk,pusk;

const uint16_t delitel [10]   PROGMEM =   {16000,1600,800,500,400,200};
const uint8_t razdelitel [10]  PROGMEM  =  {160,16,8,5,4,2};
const int8_t chastota [10]    PROGMEM  =  {1,10,20,32,40,80};

ISR ( INT1_vect )        // прерывания от энкодера
{
	if (ENCPOVOROT)      // если второй пин энкодера уже сработал то мы поворачиваем в право
	{
		switch (menu){
			case 0:
			if (pwm > 0) pwm --;
			piknut (1,50);
			break;
			case 1:
			if (nastroika_chastoti > 0) nastroika_chastoti --;
			piknut (1,50);
			break;
			case 2:
			zvuk = false;
			piknut (1,50);
			break;
		}
	}
	else                                        // если сработало прерывание от энкодера а второй пин еще не нажат то мы крутимся влево
	{
		switch (menu){
			case 0:
			if (pwm < 100) pwm ++;
			piknut (1,50);
			break;
			case 1:
			if (nastroika_chastoti < 5) nastroika_chastoti ++;
			piknut (1,50);
			break;
			case 2:
			zvuk = true;
			piknut (1,50);
			break;
		}
		
	}
}

ISR (TIMER0_OVF_vect){
	
	simvol ++;
	switch (simvol)
	{
		case 1 :
		Q3ON;
		indikciya (razr3);
		Q1OFF;
		break;
		case 2 :
		Q1ON;
		indikciya (razr2);
		Q2OFF;
		break;
		case 3 :
		Q2ON;
		if (menu == 0 ) indikciya (razr1);
		if (menu == 1 ) indikciya ('H');
		if (menu == 2 ) indikciya ('S');
		Q3OFF;
		break;
	}
	if (simvol == 3) simvol = 0;
	
	if (KNOPKAENC){             // отслеживаем кнопку енкодера
		knopkainkrement ++;     // если нажата инкрементируем
		if (knopkainkrement >= 210) knopkainkrement = 210;      // если значение больше 210 сделать 210 для защиты от переполнения переменной
		if (knopkainkrement == 200) piknut (1,500);             // если значение стало 200 пикнуть, чтоб дать понять удержание кнопки
	}
	else                                                        // если нажатия нету
	{
		if (knopkainkrement >= 20 && knopkainkrement < 200) {   // проверяем переменную кнопки на количество. если больше 20 и меньше 200
			nazhatie_knop = true;                               // и мы находимся в режиме работы то это нажатие на кнопку
			piknut (1,50);                                      // пикнуть
		}
		
		if (knopkainkrement > 200){                             // если переменная кнопки выше 200
			yderzhanie_knop = true;                             // считаем это удержанием
		}
		knopkainkrement = 0;                                    // обнуляем переменную кнопки
	}
}
ISR (TIMER2_OVF_vect){
	if (!zvuk) pik_pik ();
}

int main(void)
{
	init();

	for (;;)
	{
		if (yderzhanie_knop){
			menu ++;
			if (menu == 3) menu = 0;
			yderzhanie_knop = false;
		}
		switch (menu)
		{
			case 0:
			if (nazhatie_knop) {
				pusk = !pusk;
				nazhatie_knop = false;
			}
			if (pusk){
				LEDON;
				razbivaem_razryad (pwm);
				OCR1A = pwm * pgm_read_byte_near(&(razdelitel [nastroika_chastoti]));
			}
			if (!pusk){
				LEDOFF;
				STOPPWM;
				razbivaem_razryad (pwm);
			}
			break;
			case 1:
			LEDOFF;
			STOPPWM;
			razbivaem_razryad (pgm_read_byte_near (& chastota [nastroika_chastoti]));
			ICR1 =  pgm_read_word_near(&(delitel [nastroika_chastoti]));
			break;
			case 2:
			LEDOFF;
			STOPPWM;
			if (!zvuk) razbivaem_razryad (0);
			if (zvuk) razbivaem_razryad (1);
			break;
		}
	}
	
}

void init (void){
	
	DDRC = 0xBF;
	DDRD = 0xE3;
	DDRB = 0x01;
	
	PORTD = 0x1C;
	//--инициализируем прерывания -- //
	EICRA = 0x08;            // прерывания по спаду
	EIMSK = 0x02;	         // разрешить прерывания INT1
	//--инициализируем таймер T0-- //
	TIMSK0=0x01;            // включаем прерывания по переполнению
	TCCR0B=0x02;            // делитель тактовой частоты на 8
	//--инициализируем таймер T1-- //
	TCCR1A = 0x82;
	TCCR1B = 0x19;
	//--инициализируем таймер T2-- //
	TIMSK2 = 0x01;            // включаем прерывания по переполнению
	TCCR2B = 0x01;            // делитель тактовой частоты на 64
	
	sei();
}
void razbivaem_razryad (unsigned int vhod_chislo)  // разбиваем число на 3 разряда
{
	razr1 = vhod_chislo/100;           // сотни
	razr2 = vhod_chislo%100/10;        // десятки
	razr3 = vhod_chislo%10;            // единицы
}
void pik_pik (void)
{
	if (pik) impuls ++;
	if (impuls >= 10000) impuls = 10000;
	if (otrabotat > 0) 	pik = true;
	if (otrabotat == 0) {
		pik = false;
		impuls = 0;
		BUZEROFF;
	}
	if (impuls == 1) BUZERON;
	if (impuls == dlitelnost) BUZEROFF;
	if (impuls == ((dlitelnost + 1) * 3)){
		otrabotat --;
		impuls = 0;
	}
}
void piknut (int kolichestvo, int dolgo)
{
	otrabotat = kolichestvo;
	dlitelnost = dolgo;
}
