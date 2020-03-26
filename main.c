/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#define F_CPU 8000000UL
#define AM 1
#define PM 2
#include<avr/interrupt.h>
#include<avr/delay.h>
#include"LIB/std_types.h"
#include"DIO_interface.h"
#include"TIM_interface.h"
#include"EIRT_interface.h"
#include "Keypad_interface.h"
#include"LCD_interface.h"

void count_up(void);
void display(void);
void change_time(void);
void change_time_24(void);
void change_time_PM(void);
void change_time_layout(void);
void display_24(void);
void display_PM(void);


static u8 seconds;
static u8 mins;
static u8 hours;
static u8 flag_time_layout = 0; // to define the last layout that was used


void main(void)
{
	/*initialization*/
	DIO_SetPortDir(PORTA,OUTPUT);
	DIO_SetPinDir(PORTB,PIN0,OUTPUT);
	DIO_SetPinDir(PORTB,PIN1,OUTPUT);
	DIO_SetPinDir(PORTB,PIN2,OUTPUT);
	DIO_SetPinDir(PORTD,PIN3,INPUT);
	DIO_SetPinVal(PORTD,PIN3,HIGH);
	DIO_SetPinDir(PORTD,PIN2,INPUT);
	DIO_SetPinVal(PORTD,PIN2,HIGH);

	LCD_inti();

	LCD_GoToXY(10,1);
	LCD_SendInt(0);
	LCD_SendInt(0);
	LCD_GoToXY(7,1);
	LCD_SendInt(0);
	LCD_SendInt(0);
	LCD_GoToXY(4,1);
	LCD_SendInt(0);
	LCD_SendInt(0);

	TIM_counter0_inti_NM();

	EIRT_inti(INT1);
	EIRT_inti(INT0);

	KEYPAD_inti();

	EIRT_INT1_ISR(change_time);
	EIRT_INT0_ISR(change_time_layout);

	/*dispatcher*/
	while(1)
	{
		TIM_counter0_delay(860,count_up); //inaccuracy due to time needed to execute the isr code
	}
}

void count_up(void)
{
	if (seconds >= 59)
	{
		seconds=0;
		mins++;
		if(mins > 59)
		{
			mins = 0;
			hours++;
			if(hours>=24)
			{
				hours=0;
			}
		}
		display();
		return;
	}
	seconds++;
	display();
}

void display(void)
{
	if(flag_time_layout==0)
	{
		display_24();
	}
	else
	{
		display_PM();
	}
}

void display_24(void)
{
	if(seconds<=9)
	{
		LCD_GoToXY(10,1);
		LCD_SendInt(0);
		LCD_GoToXY(11,1);
		LCD_SendInt(seconds);
	}
	else
	{
		LCD_GoToXY(10,1);
		LCD_SendInt(seconds);
	}

	if(mins<=9)
	{
		LCD_GoToXY(7,1);
		LCD_SendInt(0);
		LCD_GoToXY(8,1);
		LCD_SendInt(mins);
	}
	else
	{
		LCD_GoToXY(7,1);
		LCD_SendInt(mins);
	}
	if(hours<=9)
	{
		LCD_GoToXY(4,1);
		LCD_SendInt(0);
		LCD_GoToXY(5,1);
		LCD_SendInt(hours);
	}
	else
	{
		LCD_GoToXY(4,1);
		LCD_SendInt(hours);
	}


}

void change_time(void)
{
	if(flag_time_layout==0)
	{
		change_time_24();
	}
	else
	{
		change_time_PM();
	}
}

void change_time_24(void)
{
	u8 temp=0;
	do{
	hours=KEYPAD_GetButton();
	}while(hours>=3);
		LCD_GoToXY(4,1);
		LCD_SendInt(hours);
	do{
		temp=KEYPAD_GetButton();
	}while(temp>=4);
		LCD_GoToXY(5,1);
		LCD_SendInt(temp);
	hours=hours*10+temp;
	do{
		mins=KEYPAD_GetButton();
	}while(mins>=6);
		LCD_GoToXY(7,1);
		LCD_SendInt(mins);
		temp=KEYPAD_GetButton();
		LCD_GoToXY(8,1);
		LCD_SendInt(temp);
		mins=mins*10+temp;
	do{
		seconds=KEYPAD_GetButton();
	}while(seconds>=6);
		LCD_GoToXY(10,1);
		LCD_SendInt(seconds);
		temp=KEYPAD_GetButton();
		LCD_GoToXY(11,1);
		LCD_SendInt(temp);
		seconds=seconds*10+temp;

}

void change_time_PM(void)
{
	u8 temp=0;
	do{
	hours=KEYPAD_GetButton();
	}while(hours>=2);
		LCD_GoToXY(4,1);
		LCD_SendInt(hours);
	do{
		temp=KEYPAD_GetButton();
	}while(temp>=3);
		LCD_GoToXY(5,1);
		LCD_SendInt(temp);
	hours=hours*10+temp;
	do{
		mins=KEYPAD_GetButton();
	}while(mins>=6);
		LCD_GoToXY(7,1);
		LCD_SendInt(mins);
		temp=KEYPAD_GetButton();
		LCD_GoToXY(8,1);
		LCD_SendInt(temp);
		mins=mins*10+temp;
	do{
		seconds=KEYPAD_GetButton();
	}while(seconds>=6);
		LCD_GoToXY(10,1);
		LCD_SendInt(seconds);
		temp=KEYPAD_GetButton();
		LCD_GoToXY(11,1);
		LCD_SendInt(temp);
		seconds=seconds*10+temp;
		do{
			temp=KEYPAD_GetButton();
		}while(temp != '+' && temp != '-');
		if(temp=='+')
		{
			LCD_GoToXY(14,1);
			LCD_SendStr("AM");
		}
		else
		{
			LCD_GoToXY(14,1);
			LCD_SendStr("PM");
			hours+=12;
		}

}



void change_time_layout(void)
{
	if(flag_time_layout==0)
	{
		flag_time_layout=1;
		if(hours==0)
		{
			LCD_GoToXY(14,1);
			LCD_SendStr("AM");
			LCD_GoToXY(4,1);
			LCD_SendInt(12);

		}
		else if (hours<=9)
		{
			LCD_GoToXY(14,1);
			LCD_SendStr("AM");
			LCD_GoToXY(4,1);
			LCD_SendInt(0);
			LCD_GoToXY(5,1);
			LCD_SendInt(hours);
		}

		else if (hours <=12)
		{
			LCD_GoToXY(14,1);
			LCD_SendStr("AM");
			LCD_GoToXY(4,1);
			LCD_SendInt(hours);
		}
		else
		{
			LCD_GoToXY(14,1);
			LCD_SendStr("PM");
			LCD_GoToXY(4,1);
			LCD_SendInt(hours-12);
		}

	}
	else if (flag_time_layout==1)
	{
		flag_time_layout=0;
		if(hours<=9)
		{
			LCD_GoToXY(4,1);
			LCD_SendInt(0);
			LCD_GoToXY(5,1);
			LCD_SendInt(hours);
		}
		else
		{
			LCD_GoToXY(4,1);
			LCD_SendInt(hours);
		}
		LCD_GoToXY(14,1);
		LCD_SendStr("  ");
	}
}

void display_PM(void)
{
	if(seconds<=9)
	{
		LCD_GoToXY(10,1);
		LCD_SendInt(0);
		LCD_GoToXY(11,1);
		LCD_SendInt(seconds);
	}
	else
	{
		LCD_GoToXY(10,1);
		LCD_SendInt(seconds);
	}

	if(mins<=9)
	{
		LCD_GoToXY(7,1);
		LCD_SendInt(0);
		LCD_GoToXY(8,1);
		LCD_SendInt(mins);
	}
	else
	{
		LCD_GoToXY(7,1);
		LCD_SendInt(mins);
	}
	if(hours==0)
	{
		LCD_GoToXY(14,1);
		LCD_SendStr("AM");
		LCD_GoToXY(4,1);
		LCD_SendInt(12);

	}
	else if (hours<=9)
	{
		LCD_GoToXY(14,1);
		LCD_SendStr("AM");
		LCD_GoToXY(4,1);
		LCD_SendInt(0);
		LCD_GoToXY(5,1);
		LCD_SendInt(hours);
	}

	else if (hours <=12)
	{
		LCD_GoToXY(14,1);
		LCD_SendStr("AM");
		LCD_GoToXY(4,1);
		LCD_SendInt(hours);
	}
	else
	{
		LCD_GoToXY(14,1);
		LCD_SendStr("PM");
		LCD_GoToXY(4,1);
		LCD_SendInt(hours-12);
	}


}
