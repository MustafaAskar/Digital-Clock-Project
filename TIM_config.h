/*
 * TIM.config.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Askar
 */

#ifndef TIM_CONFIG_H_
#define TIM_CONFIG_H_

/*
 * Watch out for timer calculations:
 * Timer/counter counts = 2^(bits of counter)
 * Timer resulation = bits of counter
 * Timer clock = System clock/Prescaller
 * Tic time = 1/Timer clock
 * Overflow total time = 2^(bits of counter) -or counter counts- * tic time
 * if desierd time < ovf time we need to calculate intial value for timer
 * timer intial value = 256 - ((desired time / overflow time) * number of counts)
 */

#include"TIM_priv.h"

/*timer clock prescaler configs*/
#define TIM_CLK_COUNTER0 CLK_256
#define TIM_CLK_COUNTER1 CLK_64
#define TIM_CLK_COUNTER2 EXTRL_CLK_FALLING

#endif /* TIM_CONFIG_H_ */
