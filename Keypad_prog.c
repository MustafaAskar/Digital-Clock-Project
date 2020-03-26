/*
 * Keypad_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#define F_CPU 8000000UL
#include"LIB/std_types.h"
#include"Keypad_config.h"
#include <avr/delay.h>
#include"DIO_interface.h"

void KEYPAD_inti()
{
	DIO_SetNippleDir(KEYPAD_PORT,KEYPAD_ROW_NIPPLE,KEYPAD_ROW_MODE);
	DIO_SetNippleDir(KEYPAD_PORT,KEYPAD_COL_NIPPLE,KEYPAD_COL_MODE);
	DIO_SetNippleVal(KEYPAD_PORT,KEYPAD_ROW_NIPPLE,KEYPAD_ROW_VAL);
	DIO_SetNippleVal(KEYPAD_PORT,KEYPAD_COL_NIPPLE,KEYPAD_COL_VAL);
}

u8 KEYPAD_GetButton()
{
	// you need to set the port to HIGH first
	u8 col=0;
	u8 row=0;
	u8 pressed_button=255;
	while(pressed_button==255)
	{
		for(row=0;row<4;row++)
		{
			DIO_SetPinVal(KEYPAD_PORT,row,LOW);
			_delay_ms(10);
			for(col=4;col<8;col++)
			{
				if(DIO_GetPinVal(KEYPAD_PORT,col)==0)
				{
					pressed_button=numbers_matrix[row][col-4];
					while(!(DIO_GetPinVal(KEYPAD_PORT,col)));
				}
			}
			DIO_SetPinVal(KEYPAD_PORT,row,HIGH);
		}
	}
	return pressed_button;
	_delay_ms(10);
}
