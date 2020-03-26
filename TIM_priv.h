/*
 * TIM_priv.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef TIM_PRIV_H_
#define TIM_PRIV_H_

/************timer registers************/
#define SREG (*((volatile u8*)0x5F))
#define OCR0 (*((volatile u8*)0x5C))
#define TIMSK (*((volatile u8*)0x59))
#define TIFR (*((volatile u8*)0x58))
#define TWCR (*((volatile u8*)0x56))
#define TCCR0 (*((volatile u8*)0x53))
#define TCNT0 (*((volatile u8*)0x52))
#define TCCR1A (*((volatile u8*)0x4F))
#define TCCR1B (*((volatile u8*)0x4E))
#define TCNT1 (*((volatile u16*)0x4C))
#define OCR1A (*((volatile u16*)0x4A))
#define OCR1B (*((volatile u16*)0x48))
#define ICR1 (*((volatile u16*)0x46))
#define TCCR2 (*((volatile u8*)0x45))
#define TCNT2 (*((volatile u8*)0x44))
#define OCR2 (*((volatile u8*)0x43))
#define ASSR (*((volatile u8*)0x42))
#define WDTCR (*((volatile u8*)0x41))


/**************registers pins************/


/******TCCR0 register*****/
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM01 3
#define COM00 4
#define COM01 5
#define WGM00 6
#define FOC0 7

/***********TCCR1A register***********/

#define WGM10 0
#define WGM11 1
#define FOC1B 2
#define FOC1A 3
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7


/***********TCCR1B register***********/

#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4
#define ICES1 6
#define ICNC1 7



/**********TIMSK register******/
#define TOIE0 0
#define OCIE0 1
#define TOIE1 2
#define OCIE1B 3
#define OCIE1A 4
#define TICIE1 5
#define TOIE2 6
#define OCIE2 7


/******* SREG register*****/
#define GI 7

/*******private configrations******/

/******prescalelr configs**********/
#define CLK_STOP 0
#define NORMAL_CLK 1
#define CLK_8 2
#define CLK_64 3
#define CLK_256 4
#define CLK_1024 5
#define EXTRL_CLK_FALLING 6
#define EXTRL_CLK_RISING 7

#endif /* TIM_PRIV_H_ */
