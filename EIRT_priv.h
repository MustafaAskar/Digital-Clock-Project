/*
 * EIRT_priv.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef EIRT_PRIV_H_
#define EIRT_PRIV_H_


/***********************************************************
 ******************* External interupt registers************
 ***********************************************************/

#define SREG 	(*((volatile u8*) 0x5F))
#define MCUCR 	(*((volatile u8*) 0x55))
#define MCUCSR 	(*((volatile u8*) 0x54))
#define GICR 	(*((volatile u8*) 0x5B))
#define GIFR 	(*((volatile u8*) 0x5A))

/****************Interrupt configs*************/
#define INT0 0
#define INT1 1
#define INT2 2
#define RISING 0
#define FALLING 1



#endif /* EIRT_PRIV_H_ */
