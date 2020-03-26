/*
 * EIRT_interface.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef EIRT_INTERFACE_H_
#define EIRT_INTERFACE_H_


void EIRT_inti(u8 INT_number); // inzilize IR0, IR1, IR2 external interuputs
void EIRT_SenseControl(u8 INT_number, u8 sense_status); //controlling interput behaviour during the runtime code
void EIRT_IntStatus(); // checking for interput status
void EIRT_GlobalInterruptEnable();
void EIRT_GlobalInterruptDisable();
void EIRT_INT0_ISR(void(*function)(void));
void EIRT_INT1_ISR(void(*function)(void));
void EIRT_INT2_ISR(void(*function)(void));



/****************Interrupt configs*************/
#define INT0 0
#define INT1 1
#define INT2 2
#define EIRT_RISING 0
#define EIRT_FALLING 1
#define EIRT_LOW_LEVEL 2
#define EIRT_TOGGLE 3


#endif /* EIRT_INTERFACE_H_ */
