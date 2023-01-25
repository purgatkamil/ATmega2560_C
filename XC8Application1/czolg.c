//#include <stdlib.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

#include "czolg.h"

void czolg_Init()
{
		   //76543210
	DDRE = 0b00111000;	//kierunki gasienic
	DDRB = 0b00010000;	//silnik lewy
	DDRH = 0b01001000;	//silnik prawy + ledy przednie
	
	DDRJ = 0b00000000; //przedni
	DDRC = 0b00000000; //tylni
	
	DDRG |= (1 << PG5);	// lewy kierunkowskaz
	DDRB |= (1 << PB6); // prawy kierunkowskaz
	
    //przednie czujniki zbli?eniowe
	PCMSK1 |= (1<<PCINT9);
	PCMSK1 |= (1<<PCINT10);
	PCIFR |= (1<<PCIF1);
	PCICR |= (1<<PCIE1);
	
	PCIFR |= (1<<PCIF0);
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT1);
}

void speed_up(int* speed_right, int* speed_left)
{
	if (*speed_right <= 230 || *speed_left <= 230)
	{
		*speed_right = *speed_right + 25;
		*speed_left = *speed_left + 25;
	}
	else
	{
		*speed_right = 255;
		*speed_left = 255;
	}
	
}

void slow_down(int* speed_right, int* speed_left)
{
	if (*speed_right >= 25 || *speed_left >= 25)
	{
		*speed_right = *speed_right - 25;
		*speed_left = *speed_left - 25;
	}
	else
	{
		*speed_right = 0;
		*speed_left = 0;
	}
}

void drive_forward(int* speed_right, int* speed_left)
{
	*speed_right = 125;
	*speed_left = 125;
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // ustawienie fastpwm
    	TCCR2B |= (1 << CS22);  //ustawienie prescalera
	PORTE &= ~(1<<PE4);	//kierunki prawej i lewej gasienicy
	PORTE |= (1<<PE5);
	turning_off();
	timers_off();
	rear_light_off();
	
}

void drive_backwards(int* speed_right, int* speed_left)
{
	*speed_right = 125;
	*speed_left = 125;
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  //ustawienie fast pwm
	TCCR2B |= (1 << CS22);  //ustawienie prescalera
	PORTE |= (1<<PE4);		//na 4 jest 1 a na 5 jest 0
	PORTE &= ~(1 << PE5);
	turning_off();
	rear_light_on();
	timers_on();
}

void turn_left(int* speed_left)
{
	*speed_left = *speed_left / 2;
}

void turn_right(int* speed_right)
{
	*speed_right = *speed_right / 2;
}

void stop()
{
	TCCR2A &= ~(1 << COM2B1);
	TCCR2A &= ~(1 << COM2A1);
	rear_light_off();
	turning_off();
	timers_off();
}

void front_light_on()
{
	PORTH |= (1<<PH3);	//swiatla przednie zapalone
}

void front_light_off()
{
	PORTH &= ~(1<<PH3);		//swiatla przednie zgaszone
}

void rear_light_on()
{
	PORTE |= (1<<PE3);
}

void rear_light_off()
{
	PORTE &= ~(1<<PE3);
}


void timers_on()
{
	TCCR0A |= (1 << COM0A1);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 255;
	TIMSK0 |= (1 << OCIE0A);
}

void timers_off()
{
	TCCR0A &= ~(1 << COM0A1);
	TCCR0B &= ~(1 << CS02) | (1 << CS00);
	OCR0A = 255;
	TIMSK0 &= ~(1 << OCIE0A);
	
}

void turning_off()
{
	PORTB &= ~(1 << PB6);
	PORTG &= ~(1 << PG5);
}
