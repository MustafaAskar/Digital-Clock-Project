/*
 * TIM_interface.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef TIM_INTERFACE_H_
#define TIM_INTERFACE_H_

/**********timer public functions***********/

/******counter 0 functions************/
void TIM_counter0_inti_NM(); // initializing counter 0 in normal mode
void TIM_counter0_inti_CTC(u8 ctc_mode); // initializing counter 0 in ctc mode
void TIM_counter0_NM_Disable(); // disable counter 0 as interval mode during the runtime
void TIM_counter0_NM_Enable(); // enable counter 0 as interval mode during the runtime
void TIM_counter0_ChangeCLK(u8 clk); // change counter 0 clock prescaler during the run time
void TIM_counter0_SetNMval(u8 value); // 0~256, Set initial value for case: desired time > overflow time
void TIM_counter0_CTC_AssignVal(u8 val); //assisgns ctc mode value to toggle on
void TIM_counter0_CTC_Enable(u8 ctc_mode); // enable counter 0 as ctc mode during the runtime
void TIM_counter0_CTC_Disable(); // disable counter 0 as ctc mode during the runtime
void TIM_counter0_inti_FAST_PWM(); // initializing counter 0 in normal mode
void TIM_counter0_FastPWM_DC(f32 duty_cycle_precentage); // counter 0 duty cycle in precentage
void TIM_counter1_inti_FAST_PWM(u16 top_val, u8 unit); // initializing counter 1 in fast pwm mode
void TIM_counter1A_CTC_AssignVal(u16 val); //assisgns ctc mode value to toggle on
void TIM_counter1B_CTC_AssignVal(u16 val); //assisgns ctc mode value to toggle on
u8 TIM_counter0_returnNMval(); // 0~256, Set initial value for case: desired time > overflow time
void TIM_counter1_SetNMval(u16 value); // 0~1024, Set initial value for case: desired time > overflow time
u16 TIM_counter1_returnNMval(); // 0~1024, Set initial value for case: desired time > overflow time
void TIM_counter1_inti_NM(); // initializing counter 1 in normal mode
void TIM_counter1_NM_Enable(); // enable counter 0 as interval mode during the runtime
void TIM_counter1_NM_Disable(); // disable counter 0 as interval mode during the runtime
void TIM_counter1_inti_CTC(u8 ctc_mode, u8 unit); //WARNING: ENABLE GLOBAL INT initializing of counter 1 in CTC mode
void TIM_counter1_InputCapture_enable(); //enabling input capture for any mode
void TIM_counter1_InputCapture_disable(); //enabling input capture for any mode
u16 TIM_counter1_InputCapture_ReturnVal(); //returns input capture value
void TIM_counter1_InputCapture_ChangeTrigEdge(u8 trig_edge); //enabling input capture for any mode
void TIM_counter1_InputCapture_EnableInterrupt(); // enable input capture unit interrupt
void TIM_counter1_InputCapture_DisableInterrupt(); // disable input capture unit interrupt
void TIM_counter1A_FastPWM_DC(f32 duty_cycle_precentage); // counter 1 duty cycle precentage unit a
void TIM_counter1B_FastPWM_DC(f32 duty_cycle_precentage); // counter 1 duty cycle precentage unit b
void TIM_counter0_delay(u16 time_delay_milliseconds, void (*function)(void));
void TIM_counter1_delay(u16 time_delay_milliseconds, void (*function)(void));
void TIM_counter2_delay(u16 time_delay_milliseconds, void (*function)(void));
void TIM_counter0_ISR(void (*function) (void));
void TIM_counter1_ISR(void (*function) (void));
void TIM_counter2_ISR(void (*function) (void));
void TIM_counter2_inti_NM(); // initializing counter 0 in normal mode
void TIM_counter2_inti_CTC(u8 ctc_mode); // initializing counter 0 in ctc mode
void TIM_counter2_NM_Disable(); // disable counter 0 as interval mode during the runtime
void TIM_counter2_NM_Enable(); // enable counter 0 as interval mode during the runtime
void TIM_counter2_SetNMval(u8 value); // 0~256, Set initial value for case: desired time > overflow time
void TIM_counter2_CTC_AssignVal(u8 val); //assisgns ctc mode value to toggle on
void TIM_counter2_inti_FAST_PWM(); // initializing counter 0 in normal mode
void TIM_counter2_FastPWM_DC(f32 duty_cycle_precentage); // counter 0 duty cycle in precentage






/*************** microcontroller configs*************/
#define TIM_COUNTER0_MAX_COUNTS 256
#define TIM_COUNTER1_MAX_COUNTS 65536
#define UNIT_A 0
#define UNIT_B 1

/*************CTC modes*************/
#define TIM_DISCONNECTED 0
#define TIM_CTC_ON_SET 1
#define TIM_CTC_ON_TOGGLE 2
#define TIM_CTC_ON_CLEAR 3

/**********input capture unit edge trig**********/
#define RISING 1
#define FALLING 0


#endif /* TIM_INTERFACE_H_ */
