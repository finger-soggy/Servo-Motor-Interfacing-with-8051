#include <reg51.h>
#include <intrins.h>
#include <stdio.h>

#define PWM_Period 18433
#define onems 922

sbit servo = P2^0;
sbit sensor1_pin = P3^2;
sbit sensor2_pin = P3^3;

unsigned int ON_Period, OFF_Period;
unsigned char angle_write, i, angle_inc;
void rotate(unsigned char);
void delay_servo(unsigned int);
void sensor1();
void sensor2();
void main() {
	servo = 0;
	sensor1_pin = 1;
	sensor2_pin = 1;
	angle_inc = 1;
	EA = 1;
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	IT1 = 1;

	TMOD = 0x01;
	
	while (1) {
		if (angle_inc == 1)
			angle_write++;
		else
			angle_write--;
		
		//----------------------------------------------------
		for (i=0; i<5; i++)
			rotate(angle_write);
		//----------------------------------------------------
		
		if (angle_write == 180) 
			angle_inc = 0;
		else if (angle_write == 0)
			angle_inc = 1;
	}
}
void sensor1() interrupt 0 {
	angle_inc = 1;
}

void sensor2() interrupt 2 {
	angle_inc = 0;
}

void rotate(unsigned char angle) {
	ON_Period = onems + angle*6;
	OFF_Period = PWM_Period - ON_Period;
	servo = 1;
	delay_servo(ON_Period);
	servo = 0;
	delay_servo(OFF_Period);
}

void delay_servo(unsigned int count) {
	unsigned int timer;
	timer = 65536 - count;
	TH0 = timer >> 8;
	TL0 = timer;
	TR0 = 1;
	while(!TF0);
	TR0 = 0;
	TF0 = 0;
}