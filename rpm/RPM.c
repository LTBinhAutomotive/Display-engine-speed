#include "stm32f4xx.h"
#include "RPM.h"

extern uint32_t encoder_count;

void GPIO_Init(void) 
{
    // Enable GPIOA clock
    //1.enable clock PORTA
		RCC->AHB1ENR &= ~(1u << 0) ;				//clear bit 0 
		RCC->AHB1ENR |=  (1u << 0) ;				//enable PORTA 
		//2.MORDER
		GPIOA->MODER &= ~(3u << 0);				  //clear bit 0 set input						
		//3.PUPDR
		GPIOA->PUPDR  &= ~(1u << 0);				//clear bit 0
}

void EXTI_Init(void)
{
		volatile unsigned int * NVIC_ISER0        = (unsigned int *)0xE000E100;
		volatile unsigned int * SYSCFG_EXTI_CR1   = (unsigned int *)0x40013808;

		/* enable CYSCFG bit 14 in register RCC*/
		RCC->APB2ENR |=  (1u << 14);
		/* config EXTI in register SYSCFG*/
		* SYSCFG_EXTI_CR1 &=  ~(15u << 0);											//PA[x] PIN		
		/* Config falling or rising register EXTI*/
		EXTI->FTSR   &= ~(1u << 0);											//clear bit 0
		EXTI->FTSR   |=  (1u << 0);											//set bit 0 falling
		/* disable IMR in register EXTI*/
		EXTI->IMR    &= ~(1u << 0);											//clear bit 0
		EXTI->IMR    |=  (1u << 0);											//set sofware
		/* enable NVIC*/
		*NVIC_ISER0   |=  (1u << 6);											//set EXTI0 of NVIC_ISER0
}

void EXTI0_IRQHandler(void) 
{
    if ((EXTI->PR & (1u << 0)) == (1u << 0))        
		{
        EXTI->PR |= (1u << 0);           // Clear the EXTI line 0 pending bit
        encoder_count++;
    }
}
