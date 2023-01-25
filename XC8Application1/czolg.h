#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


void czolg_Init();

void speed_up(int* speed_right, int* speed_left);

void slow_down(int* speed_right, int* speed_left);

void drive_forward(int* speed_right, int* speed_left);

void drive_backwards(int* speed_right, int* speed_left);

void turn_left(int* speed_left);

void turn_right(int* speed_right);

void stop();

void front_light_on();

void front_light_off();

void rear_light_on();

void rear_light_off();

void timers_on();

void timers_off();

void turning_off();

void buzzer_on();

void buzzer_off();

void buzzer_timer();