/*
 * Keypad_config.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

/**************keypad configs*************/
/*
 * note: making a pin INPUT and HIGH will activate pull up resistor in avr microcontroller
 */

#define KEYPAD_PORT PORTC


#define KEYPAD_COL_NIPPLE HIGH // high/low nibble connected to coloumn
#define KEYPAD_COL_MODE INPUT // coloumn mode
#define KEYPAD_COL_VAL HIGH // coloumn val

#define KEYPAD_ROW_NIPPLE LOW // high/low nibble connected to row
#define KEYPAD_ROW_MODE OUTPUT // row mode
#define KEYPAD_ROW_VAL HIGH // row val

u8 numbers_matrix[4][4]=
{
			{7,8,9,'/'},
			{4,5,6,'*'},
			{1,2,3,'-'},
			{'#',0,'=','+'}
};


#endif /* KEYPAD_CONFIG_H_ */
