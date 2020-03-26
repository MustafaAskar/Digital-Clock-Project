/*
 * EIRT_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#include "LIB/std_types.h"
#include "LIB/bit_math.h"
#include <avr/interrupt.h>
#include "EIRT_priv.h"
#include "EIRT_interface.h"

void (*INT0_ISR)(void) = '\0';
void (*INT1_ISR)(void) = '\0';
void (*INT2_ISR)(void) = '\0';

void EIRT_inti(u8 INT_number) // initialization IR0, IR1, IR2 external interrupts
{
	EIRT_GlobalInterruptEnable();

	//enabling IO0, IO1, IO2
	switch(INT_number)
	{
	case INT1:
		SET_BIT(GICR,7); // IR1 enabling pin
		break;
	case INT0:
		SET_BIT(GICR,6); // IR0 enabling pin
		break;
	case INT2:
		SET_BIT(GICR,5); // IR2 enabling pin
		break;
	default:
		SET_BIT(GICR,6); // IR0 enabling pin
	}

	//configuring sense control, default is with rising edge
	switch(INT_number)
	{
	case INT1:
		SET_BIT(MCUCR,2); //IR 1 rising edge
		SET_BIT(MCUCR,3);
		break;
	case INT0:
		SET_BIT(MCUCR,0); //IR0 rising edge
		SET_BIT(MCUCR,1);
		break;
	case INT2:
		SET_BIT(MCUCSR,6); //IRT2 rising edge
		break;
	default:
		SET_BIT(MCUCR,0); //IR0 rising edge
		SET_BIT(MCUCR,1);
	}
}

void EIRT_SenseControl(u8 INT_number, u8 sense_status) //controlling interput behaviour during the runtime code
{
	switch(INT_number)
	{
	case INT0:
		switch(sense_status)
		{
		case EIRT_RISING:
			SET_BIT(MCUCR,0); //IR0 rising edge
			SET_BIT(MCUCR,1);
			break;
		case EIRT_FALLING:
			SET_BIT(MCUCR,1);
			CLEAR_BIT(MCUCR,0); //IR0 falling edge
			break;
		case EIRT_LOW_LEVEL:
			CLEAR_BIT(MCUCR,0); //IR0 low level
			CLEAR_BIT(MCUCR,1);
			break;
		case EIRT_TOGGLE:
			SET_BIT(MCUCR,0); //IR0 toggle with any logical change level
			CLEAR_BIT(MCUCR,1);
			break;
		}
		break;
		case INT1:
		switch(sense_status)
		{
		case EIRT_RISING:
			SET_BIT(MCUCR,2); //IR1 rising edge
			SET_BIT(MCUCR,3);
			break;
		case EIRT_FALLING:
			SET_BIT(MCUCR,3);
			CLEAR_BIT(MCUCR,2); //IR1 falling edge
			break;
		case EIRT_LOW_LEVEL:
			CLEAR_BIT(MCUCR,2); //IR1 low level
			CLEAR_BIT(MCUCR,3);
			break;
		case EIRT_TOGGLE:
			SET_BIT(MCUCR,2); //IR0 toggle with any logical change level
			CLEAR_BIT(MCUCR,3);
			break;

		}
		break;
		case INT2:
		switch(sense_status)
		{
		case EIRT_RISING:
			SET_BIT(MCUCSR,6); //IRT2 rising edge
			break;
		case EIRT_FALLING:
			CLEAR_BIT(MCUCSR,6); //IRT2 falling edge
			break;
		}
		break;
	}
}
void EIRT_IntStatus() // checking for interrupt status
{

}

void EIRT_GlobalInterruptEnable()
{
	//enabling global interrupt pin
	SET_BIT(SREG,7);
}

void EIRT_GlobalInterruptDisable()
{
	//enabling global interrupt pin
	CLEAR_BIT(SREG,7);
}

void EIRT_INT0_ISR(void(*function)(void))
{
	INT0_ISR=function;
}

void EIRT_INT1_ISR(void(*function)(void))
{
	INT1_ISR=function;
}

void EIRT_INT2_ISR(void(*function)(void))
{
	INT2_ISR=function;
}

ISR(INT0_vect)
{
	(INT0_ISR)();
}

ISR(INT1_vect)
{
	(INT1_ISR)();
}

ISR(INT2_vect)
{
	(INT2_ISR)();
}
