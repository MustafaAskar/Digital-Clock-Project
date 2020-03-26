/*
 * LCD_config.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

/**************************************************************
**************** LCD ports config *****************************
**************************************************************/

#define LCD_DATA_PORT PORTA
#define LCD_CONTROL_PORT PORTB
#define RS PIN0
#define R_W PIN1
#define E PIN2

/**************************************************************
**************** LCD bit mode *********************************
**************************************************************/
// 1 for 8 bit mode and 0 for 4 bit mode
#define BIT_MODE 1

#endif /* LCD_CONFIG_H_ */
