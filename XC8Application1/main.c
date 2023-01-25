#define F_CPU 16000000
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "uart.h"
#include "lcd.h"
#include "czolg.h"

/* Define UART buad rate here */
#define UART_BAUD_RATE      115200 

int speed_left = 255;
int speed_right = 255;

volatile int swiatla = 0;
volatile int skrecanie = 0;

int main(void)
{
	czolg_Init();
	sei();
	
    unsigned int c;
    char buffer[7];
    int num=134;
	char napis[10];
	

	lcd_init(LCD_DISP_ON);    // init lcd and turn on

    
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
    
    uart_puts("String stored in SRAM\n");
    
    uart_puts_P("String stored in FLASH\n");
       
	sprintf( buffer, "%d", num);   // convert interger into string (decimal format)            
    uart_puts(buffer);        // and transmit string to UART

    uart_putc('\r');
    
    for(;;){
		
		OCR2B = speed_left;
		OCR2A = speed_right;
				
        c = uart_getc();
        if ( c & UART_NO_DATA ){

        }
        else{

            if ( c & UART_FRAME_ERROR )
                uart_puts_P("UART Frame Error: ");
				
            if ( c & UART_OVERRUN_ERROR )
                uart_puts_P("UART Overrun Error: ");
				
            if ( c & UART_BUFFER_OVERFLOW )
                uart_puts_P("Buffer overflow error: ");
			
			switch (c){
				case 's':
					drive_backwards(&speed_right, &speed_left);
					break;
					
				case 'w':
					drive_forward(&speed_right, &speed_left);
					break;	
				case 'a':
					turn_left(&speed_left);
					turning_off();
					timers_off();
					timers_on();
					skrecanie = 0;
					break;
				case 'd':
					turn_right(&speed_right);
					turning_off();
					timers_off();
					timers_on();
					skrecanie = 1;
					break;
					
				case 'x':
					stop();					
					break;
					
				case 'l':
					front_light_on();
					break;
					
				case 'k':
					front_light_off();
					break;

                case 'i':
                    speed_up(&speed_right, &speed_left);
					break;
					
				case 'o':
					slow_down(&speed_right, &speed_left);
					break;
			}
		lcd_home();
		sprintf(napis, "%d", speed_right);
		lcd_puts(napis);
		lcd_puts("  ");
        }

    }
    
}
ISR (PCINT1_vect){
	OCR2A = 0;
	OCR2B = 0;
}

ISR(TIMER0_COMPA_vect){
	if(skrecanie == 0){
			if (swiatla<25){
				PORTG &= ~(1 << PG5);
			}
			else{
				PORTG |= (1 << PG5);
			}
			swiatla++;
			if (swiatla>50) swiatla = 0;
	}
	else if(skrecanie == 1){
			if (swiatla<25){
				PORTB &= ~(1 << PB6);
			}
			else{
				PORTB |= (1 << PB6);
			}
			swiatla++;
			if (swiatla>50) swiatla = 0;
			
	}
}

	


