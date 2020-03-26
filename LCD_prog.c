/*
 * LCD_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#define F_CPU 8000000UL
#include"LIB/std_types.h"
#include"LIB/bit_math.h"
#include"stdlib.h" //this library includes the itoa function
#include"DIO_interface.h"
#include"LCD_interface.h"
#include"LCD_config.h"
#include <avr/delay.h>

#if BIT_MODE
void LCD_inti() //intilization LCD
{
	//setting DIO intilization
	DIO_SetPinDir(LCD_CONTROL_PORT,E,OUTPUT);
	DIO_SetPinDir(LCD_CONTROL_PORT,R_W,OUTPUT);
	DIO_SetPinDir(LCD_CONTROL_PORT,RS,OUTPUT);
	DIO_SetPortDir(LCD_DATA_PORT,OUTPUT);

	//delay more than 30 ms to ensure the initialization of lcd
	_delay_ms(30);
	//8 bit mode
	LCD_SendCMD(LCD_FUNCSET_8BIT);
	_delay_ms(1);
	//Display on with curses on
	LCD_SendCMD(LCD_DISON_CURSOFF);
	_delay_ms(1);
	//Clear screen
	LCD_SendCMD(LCD_CLR);
	_delay_ms(2);
	//go into entry mode
	LCD_SendCMD(LCD_ENTRYMODE);
	_delay_ms(2);
	LCD_SendCMD(LCD_FIRSTLINE);
	_delay_ms(2);

	}


void LCD_SendChar(u8 character) // Send and display a character on LCD
{
	/*Set RS*/
	DIO_SetPinVal(LCD_CONTROL_PORT,RS,HIGH); //RS to PINA0, HIGH for character and LOW for command

	/*Reset R/W */
	DIO_SetPinVal(LCD_CONTROL_PORT,R_W,LOW); //R/W to PINA1, LOW to write and HIGH to read

	/*Set E to high*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH); // E to PINA2

	/*Load command on data pins*/
	DIO_AssignPortVal(LCD_DATA_PORT,character); //Data pins connected to port b


	/*Set E to low*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,LOW);

	/*Set E to high again to fetch a character*/
	_delay_ms(5);

	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH);
	_delay_ms(2);
}

void LCD_SendCMD (u8 CMD) // Send and execute a command on LCD
{
	/*Reset RS*/
	DIO_SetPinVal(LCD_CONTROL_PORT,RS,LOW); //RS to PINA0, HIGH for character and LOW for command

	/*Reset R/W */
	DIO_SetPinVal(LCD_CONTROL_PORT,R_W,LOW); //R/W to PINA1, LOW to write and HIGH to read
	_delay_ms(2);

	/*Set E to high*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH); // E to PINA2
	_delay_ms(2);

	/*Load command on data pins*/
	DIO_AssignPortVal(LCD_DATA_PORT,CMD); //Data pins
	_delay_ms(2);

	/*Set E to low*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,LOW);
	_delay_ms(2);

	/*Set E to high again to fetch a command*/
	_delay_ms(5);
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH);
	_delay_ms(2);
}

void LCD_GoToXY(u8 character, u8 line) // Move cursor to X character Y line
{
	switch(line)
	{
	case 1:
		LCD_SendCMD(0x80 + character);
		_delay_ms(2); //this delay is for the microcontroller to free the busy bit in LCD!!!!!
		break;
	case 2:
		LCD_SendCMD(0xC0 + character);
		_delay_ms(2); //this delay is for the microcontroller to free the busy bit in LCD!!!!!
		break;
	}
}

void LCD_SendStr(u8 * str) //Send string to LCD
{
	u8 counter=0;
	while(str[counter]!='\0') //Null termenatior at the end of every string
	{
		LCD_SendChar(str[counter]);
		_delay_ms(5);
		counter++;
	}
}

void LCD_CustomCharacter(u8* character_pattern, u8 character_indx) // to design and store a custom character
{
	u8 counter;
	LCD_SendCMD(0x40+(8*character_indx)); // 0x40 first location in CGRAM for first character
	_delay_ms(1);
	for(counter=0;counter<LCD_CustomCharacterSize;counter++)
	{
		LCD_SendChar(character_pattern[counter]); // character pixels pattern
		_delay_ms(1);
	}
}


void LCD_SendInt(u16 int_number) //this function to print direct integre number on lcd
{
	u8 str_number[5]={10,10,10,10,10};
	u8 counter=4;
	if(int_number==0)
	{
		str_number[counter]=int_number%10;
		int_number/=10;
		counter--;
	}
	while(int_number>0)
	{
		str_number[counter]=int_number%10;
		int_number/=10;
		counter--;
	}
	for(counter=0;counter<5;counter++)
	{
		if(str_number[counter]<=9)
		{
			LCD_SendChar((str_number[counter]+48));
		}
	}
}

#else

void LCD_inti() //intilization LCD
{
	//setting DIO intilization
	DIO_SetPinDir(LCD_CONTROL_PORT,E,OUTPUT);
	DIO_SetPinDir(LCD_CONTROL_PORT,R_W,OUTPUT);
	DIO_SetPinDir(LCD_CONTROL_PORT,RS,OUTPUT);
	DIO_SetPinDir(LCD_DATA_PORT,PIN7,OUTPUT);
	DIO_SetPinDir(LCD_DATA_PORT,PIN6,OUTPUT);
	DIO_SetPinDir(LCD_DATA_PORT,PIN5,OUTPUT);
	DIO_SetPinDir(LCD_DATA_PORT,PIN4,OUTPUT);

	//delay more than 30 ms to ensure the initialization of lcd
	_delay_ms(35);

	//4 bit mode
	LCD_SendCMD(LCD_FUNCSET_4BIT);
	_delay_ms(2);

	//Display on with curses on
	LCD_SendCMD(LCD_DISPON_CURSON);
	_delay_ms(2);

	//Set LCD to home
	LCD_SendCMD(LCD_HOME);
	_delay_ms(2);

	//Clear screen
	LCD_SendCMD(LCD_CLR);
	_delay_ms(2);

	//go into entry mode
	LCD_SendCMD(LCD_ENTRYMODE);
	_delay_ms(2);
	}

void LCD_SendChar(u8 character) // Send and display a character on LCD
{
	/*Set RS*/
	DIO_SetPinVal(LCD_CONTROL_PORT,RS,HIGH); //HIGH for character and LOW for command

	/*Reset R/W */
	DIO_SetPinVal(LCD_CONTROL_PORT,R_W,LOW); //LOW to write and HIGH to read

	/*Load character on data pins*/
	DIO_AssignNippleVal(LCD_DATA_PORT,HIGH,character);

	/*Set E to high*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH);
	_delay_ms(1);

	/*Set E to low*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,LOW);

	/* sending second nipple data*/
	character<<=4;
	DIO_AssignNippleVal(LCD_DATA_PORT,HIGH,character);

	/*Set E to high again to fetch a cmd*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH);
	_delay_ms(1);

	/*Set E to low*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,LOW);

	_delay_ms(2);

}

void LCD_SendCMD(u8 cmd) // Send and display a cmd on LCD
{
	/*Set RS*/
	DIO_SetPinVal(LCD_CONTROL_PORT,RS,LOW); //HIGH for character and LOW for command

	/*Reset R/W */
	DIO_SetPinVal(LCD_CONTROL_PORT,R_W,LOW); //LOW to write and HIGH to read

	/*Load command on data pins*/
	DIO_AssignNippleVal(LCD_DATA_PORT,HIGH,cmd);

	/*Set E to high*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH);
	_delay_ms(1);

	/*Set E to low*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,LOW);

	/* sending second nipple data*/
	cmd<<=4;
	DIO_AssignNippleVal(LCD_DATA_PORT,HIGH,cmd);

	/*Set E to high again to fetch a cmd*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,HIGH);
	_delay_ms(1);

	/*Set E to low*/
	DIO_SetPinVal(LCD_CONTROL_PORT,E,LOW);

	_delay_ms(2);
}

void LCD_GoToXY(u8 character, u8 line) // Move cursor to X character Y line
{
	switch(line)
	{
	case 1:
		LCD_SendCMD(0x80 + character);
		_delay_ms(5); //this delay is for the microcontroller to free the busy bit in LCD!!!!!
		break;
	case 2:
		LCD_SendCMD(0xC0 + character);
		_delay_ms(5); //this delay is for the microcontroller to free the busy bit in LCD!!!!
		break;
	}
}

void LCD_SendStr(u8 * str) //Send string to LCD
{
	u8 counter=0;
	while(str[counter]!='\0') //Null termenatior at the end of every string
	{
		LCD_SendChar(str[counter]); //this delay is for the microcontroller to free the busy bit in LCD!!!!!
		_delay_ms(1); //this delay is for the microcontroller to free the busy bit in LCD!!!!
		counter++;
	}
}

void LCD_CustomCharacter(u8* character_pattern, u8 character_indx)
{
	u8 counter;
	LCD_SendCMD(0x40+(8*character_indx));
	_delay_ms(1);
	for(counter=0;counter<LCD_CustomCharacterSize;counter++)
	{
		LCD_SendChar(character_pattern[counter]);
		_delay_ms(1);
	}
}

void LCD_SendInt(u16 int_number) //this function to print direct integre number on lcd
{
	u8 str_number[5]={10,10,10,10,10};
	u8 counter=4;
	if(int_number==0)
	{
		str_number[counter]=int_number%10;
		int_number/=10;
		counter--;
	}
	while(int_number>0)
	{
		str_number[counter]=int_number%10;
		int_number/=10;
		counter--;
	}
	for(counter=0;counter<5;counter++)
	{
		if(str_number[counter]<=9)
		{
			LCD_SendChar((str_number[counter]+48));
		}
	}

}


#endif
