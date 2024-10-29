
#include "DL.h"
#include "RCC.h"

void TIM9Config (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable Timer clock
	2. Set the prescalar and the ARR
	3. Enable the Timer, and wait for the update Flag to set
	************************************************/

// 1. Enable Timer clock
	RCC->APB2ENR |= (1<<16);  // Enable the timer6 clock
	
// 2. Set the prescalar and the ARR
	TIM9->PSC = 90-1;  // 90MHz/90 = 1 MHz ~~ 1 uS delay
	TIM9->ARR = 0xFFFF;  // MAX ARR value
	
// 3. Enable the Timer, and wait for the update Flag to set
	TIM9->CR1 |= (1<<0); // Enable the Counter
	while (!(TIM9->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
}

void Delay_us (uint16_t us)
{
	/************** STEPS TO FOLLOW *****************
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value. As each count will take 1 us, 
		 the total waiting time will be the required us delay
	************************************************/
	TIM9->CNT = 0;
	while (TIM9->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}
