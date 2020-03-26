/*
 * TIM_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef F_CPU
/* prevent compiler error by supplying a default */
# warning "F_CPU not defined for timer delay"
# define F_CPU 1000000UL
#endif


#include"LIB/std_types.h"
#include"LIB/bit_math.h"
#include"TIM_priv.h"
#include"TIM_interface.h"
#include"TIM_config.h"
#include<avr/interrupt.h>

static u16 timer0_overflow_number=0;
static u32 timer0_tic_number=128;
static u16 timer1_overflow_number=0;
static u32 timer1_tic_number=0;
static u16 timer2_overflow_number=0;
static u32 timer2_tic_number=128;

//getting prescaler from config file
#if TIM_CLK_COUNTER0 == CLK_8
	static u16 prescaler = 8;
#elif TIM_CLK_COUNTER0 == CLK_64
	static u16 prescaler = 64;
#elif TIM_CLK_COUNTER0== CLK_256
	static u16 prescaler = 256;
#elif TIM_CLK_COUNTER0 == CLK_1024
	static u16 prescaler = 1024;
#endif

#define TICKS (prescaler*1000000/(F_CPU))

void (*timer0_ISR) (void) = '\0';
void (*timer1_ISR) (void) = '\0';
void (*timer2_ISR) (void) = '\0';


void TIM_counter0_inti_NM() //WARNING: ENABLE GLOBAL INT initializing counter 0 in normal mode
{
	// status register general interrupt enable
	SET_BIT(SREG,GI);

	CLEAR_BIT(TCCR0,WGM01);
	CLEAR_BIT(TCCR0,WGM00);

	// initialization of timer clock (prescaler) in counter 0
	TCCR0&=0b11111000;
	TCCR0|=(TIM_CLK_COUNTER0&(0b00000111));

	//counter 0 interrupt pin
	SET_BIT(TIMSK,TOIE0);

	//output compare capture interrupt disable
	CLEAR_BIT(TIMSK,1);
}


void TIM_counter0_inti_CTC(u8 ctc_mode) // WARNING: ENABLE GLOBAL INT, initializing counter 0 in ctc mode
{
	//CTC mode
	SET_BIT(TCCR0,WGM01);
	CLEAR_BIT(TCCR0,WGM00);

	// status register general interrupt enable
	CLEAR_BIT(SREG,GI);

	// initialization of timer clock (prescaler) in counter 0
	TCCR0&=0b11111000;
	TCCR0|=(TIM_CLK_COUNTER0&(0b00000111));

	//output compare capture interrupt disable
	CLEAR_BIT(TIMSK,1);

	switch(ctc_mode)
	{
	case TIM_DISCONNECTED:
		//stop ctc mode
		CLEAR_BIT(TCCR0,COM00);
		CLEAR_BIT(TCCR0,COM01);
		break;
	case TIM_CTC_ON_SET:
		//set on compare match
		SET_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	case TIM_CTC_ON_CLEAR:
		//clear on compare match
		CLEAR_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	case TIM_CTC_ON_TOGGLE:
		//toggle on compare match
		SET_BIT(TCCR0,COM00);
		CLEAR_BIT(TCCR0,COM01);
		break;
	}
}

void TIM_counter0_inti_FAST_PWM() // initializing counter 0 in normal mode
{
	//FAST PWM mode
	SET_BIT(TCCR0,WGM01);
	SET_BIT(TCCR0,WGM00);

	// initialization of timer clock (prescaler) in counter 0
	TCCR0&=0b11111000;
	TCCR0|=(TIM_CLK_COUNTER0&(0b00000111));

	//non inverted fast pwm mode
	SET_BIT(TCCR0,COM01);
	CLEAR_BIT(TCCR0,COM00);

	//disable counter 0 interrupt pin
	CLEAR_BIT(TIMSK,TOIE0);
}

void TIM_counter1_inti_NM() //WARNING: ENABLE GLOBAL INT initializing of counter 1 in normal mode
{

	//choosing normal mode
	CLEAR_BIT(TCCR1B,WGM13);
	CLEAR_BIT(TCCR1B,WGM12);
	CLEAR_BIT(TCCR1A,WGM11);
	CLEAR_BIT(TCCR1A,WGM10);

	// initialization of timer clock (prescaler) in counter 1
	TCCR1B&=0b11111000;
	TCCR1B|=(TIM_CLK_COUNTER1&(0b00000111));

	//counter 1 interrupt pin
	SET_BIT(TIMSK,TOIE1);

	//output compare capture and input capture unit interrupt disable
	CLEAR_BIT(TIMSK,TICIE1);
	CLEAR_BIT(TIMSK,OCIE1A);
	CLEAR_BIT(TIMSK,OCIE1B);
}

void TIM_counter1_inti_CTC(u8 ctc_mode, u8 unit) //WARNING: ENABLE GLOBAL INT initializing of counter 1 in CTC mode
{
	//choosing normal mode
	CLEAR_BIT(TCCR1B,WGM13);
	SET_BIT(TCCR1B,WGM12);
	CLEAR_BIT(TCCR1A,WGM11);
	CLEAR_BIT(TCCR1A,WGM10);

	// initialization of timer clock (prescaler) in counter 1
	TCCR1B&=0b11111000;
	TCCR1B|=(TIM_CLK_COUNTER1&(0b00000111));

	//counter 1 interrupt pin
	SET_BIT(TIMSK,TOIE1);

	//output compare capture and input capture unit interrupt disable
	CLEAR_BIT(TIMSK,TICIE1);
	CLEAR_BIT(TIMSK,OCIE1A);
	CLEAR_BIT(TIMSK,OCIE1B);

switch(unit)
{
case UNIT_A:
	{
			switch(ctc_mode)
		{
		case TIM_DISCONNECTED:
			//stop ctc mode
			CLEAR_BIT(TCCR1A,COM1A0);
			CLEAR_BIT(TCCR1A,COM1A1);
			break;
		case TIM_CTC_ON_SET:
			//set on compare match
			SET_BIT(TCCR1A,COM1A0);
			SET_BIT(TCCR1A,COM1A1);
			break;
		case TIM_CTC_ON_CLEAR:
			//clear on compare match
			CLEAR_BIT(TCCR1A,COM1A0);
			SET_BIT(TCCR1A,COM1A1);
			break;
		case TIM_CTC_ON_TOGGLE:
			//toggle on compare match
			SET_BIT(TCCR1A,COM1A0);
			CLEAR_BIT(TCCR1A,COM1A1);
			break;
		}
	} break;
	case UNIT_B:
	{
			switch(ctc_mode)
		{
		case TIM_DISCONNECTED:
			//stop ctc mode
			CLEAR_BIT(TCCR1A,COM1A0);
			CLEAR_BIT(TCCR1A,COM1A1);
			break;
		case TIM_CTC_ON_SET:
			//set on compare match
			SET_BIT(TCCR1A,COM1A0);
			SET_BIT(TCCR1A,COM1A1);
			break;
		case TIM_CTC_ON_CLEAR:
			//clear on compare match
			CLEAR_BIT(TCCR1A,COM1A0);
			SET_BIT(TCCR1A,COM1A1);
			break;
		case TIM_CTC_ON_TOGGLE:
			//toggle on compare match
			SET_BIT(TCCR1A,COM1A0);
			CLEAR_BIT(TCCR1A,COM1A1);
			break;
		}
	} break;
}
}

void TIM_counter1_inti_FAST_PWM(u16 top_val, u8 unit) // initializing counter 1 in fast pwm mode
{
	//FAST PWM mode
	SET_BIT(TCCR1B,WGM13);
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1A,WGM11);
	CLEAR_BIT(TCCR1A,WGM10);

	// initialization of timer clock (prescaler) in counter 0
	TCCR1B&=0b11111000;
	TCCR1B|=(TIM_CLK_COUNTER1&(0b00000111));

	//non inverted fast pwm mode
	switch(unit)
	{
	case UNIT_A:
		{
			SET_BIT(TCCR1A,COM1A1);
			CLEAR_BIT(TCCR1A,COM1A0);
		} break;
		case UNIT_B:
		{
			SET_BIT(TCCR1A,COM1B1);
			CLEAR_BIT(TCCR1A,COM1B0);
		} break;
	}

	//setting ICR1_val
	ICR1=top_val;
}

void TIM_counter2_inti_NM() //WARNING: ENABLE GLOBAL INT initializing counter 0 in normal mode
{
	// status register general interrupt enable
	SET_BIT(SREG,GI);

	CLEAR_BIT(TCCR2,WGM21);
	CLEAR_BIT(TCCR2,WGM20);


	// initialization of timer clock (prescaler) in counter 2
	TCCR2&=0b11111000;
	TCCR2|=(TIM_CLK_COUNTER2&(0b00000111));

	//counter 0 interrupt pin
	SET_BIT(TIMSK,TOIE2);

	//output compare capture interrupt disable
	CLEAR_BIT(TIMSK,OCIE2);
}


void TIM_counter2_inti_CTC(u8 ctc_mode) // WARNING: ENABLE GLOBAL INT, initializing counter 0 in ctc mode
{
	//CTC mode
	SET_BIT(TCCR2,WGM01);
	CLEAR_BIT(TCCR2,WGM00);

	// status register general interrupt enable
	CLEAR_BIT(SREG,GI);

	// initialization of timer clock (prescaler) in counter 0
	TCCR2&=0b11111000;
	TCCR2|=(TIM_CLK_COUNTER0&(0b00000111));

	//output compare capture interrupt disable
	CLEAR_BIT(TIMSK,OCIE2);

	switch(ctc_mode)
	{
	case TIM_DISCONNECTED:
		//stop ctc mode
		CLEAR_BIT(TCCR2,COM20);
		CLEAR_BIT(TCCR2,COM21);
		break;
	case TIM_CTC_ON_SET:
		//set on compare match
		SET_BIT(TCCR2,COM20);
		SET_BIT(TCCR2,COM21);
		break;
	case TIM_CTC_ON_CLEAR:
		//clear on compare match
		CLEAR_BIT(TCCR2,COM20);
		SET_BIT(TCCR2,COM21);
		break;
	case TIM_CTC_ON_TOGGLE:
		//toggle on compare match
		SET_BIT(TCCR2,COM20);
		CLEAR_BIT(TCCR2,COM21);
		break;
	}
}

void TIM_counter2_inti_FAST_PWM() // initializing counter 0 in normal mode
{
	//FAST PWM mode
	SET_BIT(TCCR2,WGM21);
	SET_BIT(TCCR2,WGM20);

	// initialization of timer clock (prescaler) in counter 0
	TCCR2&=0b11111000;
	TCCR2|=(TIM_CLK_COUNTER0&(0b00000111));

	//non inverted fast pwm mode
	SET_BIT(TCCR2,COM21);
	CLEAR_BIT(TCCR2,COM20);

	//disable counter 0 interrupt pin
	CLEAR_BIT(TIMSK,TOIE2);
}


void TIM_counter0_NM_Disable() // disable counter 0 as interval mode during the runtime
{
	TCCR0&=0b11111000; //initialization of timer clock (prescaler) in counter 0
}


void TIM_counter0_NM_Enable() // enable counter 0 as interval mode during the runtime
{
	TIM_counter0_SetNMval(0);
	TCCR0&=0b11111000;
	TCCR0|=(TIM_CLK_COUNTER0&(0b00000111));
}


void TIM_counter1_NM_Disable() // disable counter 1 as interval mode during the runtime
{
	TCCR1B&=0b11111000; //initialization of timer clock (prescaler) in counter 0
}


void TIM_counter1_NM_Enable() // enable counter 1 as interval mode during the runtime
{
	TIM_counter1_SetNMval(0);
	TCCR1B&=0b11111000;
	TCCR1B|=(TIM_CLK_COUNTER1&(0b00000111));
}

void TIM_counter2_NM_Disable() // disable counter 0 as interval mode during the runtime
{
	TCCR2&=0b11111000; //initialization of timer clock (prescaler) in counter 0
}


void TIM_counter2_NM_Enable() // enable counter 0 as interval mode during the runtime
{
	TIM_counter2_SetNMval(0);
	TCCR2&=0b11111000;
	TCCR2|=(TIM_CLK_COUNTER2&(0b00000111));
}



void TIM_counter0_CTC_Enable(u8 ctc_mode) // enable counter 0 as ctc mode during the runtime
{
	switch(ctc_mode)
	{
	case TIM_CTC_ON_SET:
		//set on compare match
		SET_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	case TIM_CTC_ON_CLEAR:
		//clear on compare match
		CLEAR_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	case TIM_CTC_ON_TOGGLE:
		//toggle on compare match
		SET_BIT(TCCR0,COM00);
		CLEAR_BIT(TCCR0,COM01);
		break;

	}
}

void TIM_counter0_CTC_Disable() // disable counter 0 as ctc mode during the runtime
{
	//stop ctc mode
	CLEAR_BIT(TCCR0,COM00);
	CLEAR_BIT(TCCR0,COM01);
}


void TIM_counter0_ChangeCLK(u8 clk) // change counter 0 clock prescaler during the run time
{
	TCCR0&=0b11111000;
	TCCR0|=(clk&(0b00000111));
}

void TIM_counter0_SetNMval(u8 value) // 0~256, Set initial value for case: desired time > overflow time
{
	TCNT0=value;
}

u8 TIM_counter0_returnNMval() // 0~256, Set initial value for case: desired time > overflow time
{
	return TCNT0;
}


void TIM_counter1_SetNMval(u16 value) // 0~65,535, Set initial value for case: desired time > overflow time
{
	TCNT1=value;
}

u16 TIM_counter1_returnNMval() // 0~65,535, Set initial value for case: desired time > overflow time
{
	return TCNT1;
}

void TIM_counter2_SetNMval(u8 value) // 0~256, Set initial value for case: desired time > overflow time
{
	TCNT2=value;
}

u8 TIM_counter2_returnNMval() // 0~256, Set initial value for case: desired time > overflow time
{
	return TCNT2;
}


void TIM_counter0_delay(u16 time_delay_milliseconds, void (*function)(void))
{
	u16 temp = 0;
	static u16 last_value=0;
	if(last_value != time_delay_milliseconds)
	{
		last_value = time_delay_milliseconds;
		timer0_tic_number=(u32)time_delay_milliseconds*1000/TICKS;
		timer0_overflow_number=(timer0_tic_number/TIM_COUNTER0_MAX_COUNTS);
		temp = timer0_overflow_number * TIM_COUNTER0_MAX_COUNTS;
		timer0_tic_number -= temp;
		TCNT0 = timer0_tic_number;
	}
	TIM_counter0_ISR(function);
}

void TIM_counter1_delay(u16 time_delay_milliseconds, void (*function)(void))
{
	u16 temp = 0;
	static u16 last_value=0;
	if(last_value != time_delay_milliseconds)
	{
		last_value = time_delay_milliseconds;
		timer1_tic_number=(u32)time_delay_milliseconds*1000/TICKS;
		timer1_overflow_number=(timer1_tic_number/TIM_COUNTER1_MAX_COUNTS);
		temp = timer1_overflow_number * TIM_COUNTER1_MAX_COUNTS;
		timer1_tic_number -= temp;
		TCNT1 = timer0_tic_number;
	}
	TIM_counter1_ISR(function);
}

void TIM_counter2_delay(u16 time_delay_milliseconds, void (*function)(void))
{
	u16 temp = 0;
	static u16 last_value=0;
	if(last_value != time_delay_milliseconds)
	{
		last_value = time_delay_milliseconds;
		timer2_tic_number=(u32)time_delay_milliseconds*1000/TICKS;
		timer2_overflow_number=(timer2_tic_number/TIM_COUNTER0_MAX_COUNTS);
		temp = timer2_overflow_number * TIM_COUNTER0_MAX_COUNTS;
		timer2_tic_number -= temp;
		TCNT2 = timer2_tic_number;
	}
	TIM_counter2_ISR(function);
}

void TIM_counter0_ISR(void (*function) (void))
{
	timer0_ISR=function;
}

void TIM_counter1_ISR(void (*function) (void))
{
	timer1_ISR=function;
}

void TIM_counter2_ISR(void (*function) (void))
{
	timer2_ISR=function;
}


void TIM_counter0_CTC_AssignVal(u8 val) //assisgns ctc mode value to toggle on
{
	OCR0=val;
}

void TIM_counter0_FastPWM_DC(f32 duty_cycle_precentage)
{

	OCR0=(duty_cycle_precentage/100.0)*(TIM_COUNTER0_MAX_COUNTS-1);
}


void TIM_counter1A_CTC_AssignVal(u16 val) //assisgns ctc mode value to toggle on
{
	OCR1A=val;
}

void TIM_counter1B_CTC_AssignVal(u16 val) //assisgns ctc mode value to toggle on
{
	OCR1B=val;
}

void TIM_counter1A_FastPWM_DC(f32 duty_cycle_precentage)
{

	OCR1A=(duty_cycle_precentage/100.0)*(TIM_COUNTER1_MAX_COUNTS-1);
}

void TIM_counter1B_FastPWM_DC(f32 duty_cycle_precentage)
{

	OCR1B=(duty_cycle_precentage/100.0)*(TIM_COUNTER1_MAX_COUNTS-1);
}

void TIM_counter2_CTC_AssignVal(u8 val) //assisgns ctc mode value to toggle on
{
	OCR2=val;
}

void TIM_counter2_FastPWM_DC(f32 duty_cycle_precentage)
{

	OCR2=(duty_cycle_precentage/100.0)*(TIM_COUNTER0_MAX_COUNTS-1);
}


void TIM_counter1_InputCapture_enable() //enabling input capture for any mode
{
	//enable input capture mode
	SET_BIT(TIMSK,TICIE1);

	//choosing edge trigger, default rising edge
	SET_BIT(TCCR1B,ICES1);
}

void TIM_counter1_InputCapture_disable() //enabling input capture for any mode
{
	//enable input capture mode
	CLEAR_BIT(TIMSK,TICIE1);
}

void TIM_counter1_InputCapture_ChangeTrigEdge(u8 trig_edge) //enabling input capture for any mode
{
	switch(trig_edge)
	{
		case RISING:
			SET_BIT(TCCR1B,ICES1);
			break;
		case FALLING:
			CLEAR_BIT(TCCR1B,ICES1);
			break;
	}
}

u16 TIM_counter1_InputCapture_ReturnVal() //returns input capture value
{
	return ICR1;
}

void TIM_counter1_InputCapture_EnableInterrupt()
{
	SET_BIT(TIFR,5); //5 is icf1 bit
}

void TIM_counter1_InputCapture_DisableInterrupt()
{
	CLEAR_BIT(TIFR,5); //5 is icf1 bit
}

ISR(TIMER0_OVF_vect)
{
	static u8 number_of_overflows=0;
	number_of_overflows++;
	if(number_of_overflows==timer0_overflow_number)
	{
		TIM_counter0_SetNMval(timer0_tic_number);
		number_of_overflows=0;
		(timer0_ISR)();
	}
}
ISR(TIMER1_OVF_vect)
{
	static u8 number_of_overflows=0;
	number_of_overflows++;
	if(number_of_overflows==timer1_overflow_number)
	{
		TIM_counter1_SetNMval(timer1_tic_number);
		number_of_overflows=0;
		(timer1_ISR)();
	}
}
ISR(TIMER2_OVF_vect)
{
	static u8 number_of_overflows=0;
	number_of_overflows++;
	if(number_of_overflows==timer2_overflow_number)
	{
		TIM_counter2_SetNMval(timer2_tic_number);
		number_of_overflows=0;
		(timer2_ISR)();
	}
}
