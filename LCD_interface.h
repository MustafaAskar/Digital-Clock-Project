/*
 * LCD_interface.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_


/* LCD macros*/
#define LCD_CLR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRYMODE 0x06
#define LCD_DISPOFF 0x08
#define LCD_DISPON_CURSON 0x0e
#define LCD_FUNCSET 0x30
#define LCD_FUNCSET_8BIT 0x38
#define LCD_FUNCSET_4BIT 0x28
#define LCD_SETCURSOR 0x80
#define LCD_DISON_CURSBLINK 0x0f
#define LCD_DISON_CURSOFF 0x0c
#define LCD_FIRSTLINE 0x80
#define LCD_CustomCharacterSize 7


/*******************************************************************
*****************************Public functions***********************
*******************************************************************/
void LCD_inti(); // LCD 8 bit mode intilization
void LCD_SendChar(u8 character); // Send and display a character on LCD
void LCD_SendCMD (u8 CMD); // Send and execute a command on LCD
void LCD_GoToXY(u8 character, u8 line); // Move cursor to X character Y line
void LCD_SendStr(u8 * str); //Send string to LCD
void LCD_CustomCharacter(u8* character_pattern, u8 character_indx); // to design and store a custom character
void LCD_SendInt(u16 int_number); //this function to print direct integre number on lcd

#endif /* LCD_INTERFACE_H_ */
