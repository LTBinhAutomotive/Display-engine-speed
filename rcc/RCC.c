

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 180
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
#include "stm32f4xx.h" 
#include "RCC.h"

void SysClockConfig (void)
{
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
	
	
	#define PLL_M 	4
	#define PLL_N 	180
	#define PLL_P 	0  // PLLP = 2

	//*1.enable HSE and wait HSE ready*/
	RCC->CR &= ~(1u << 16);													//clear bit 16
	RCC->CR |=  (1u << 16);   											//enable HSE ON
	while ( !((RCC->CR) & (1u << 17)) );  					// wait HSE ready
	
	/*2.set POWER enable clock and volture regulator*/
	RCC->APB1ENR &= ~(1u << 28);										//clear bit 28
	RCC->APB1ENR |=  (1u << 28);										//set enable POWER
	PWR->CR      &= ~(3u << 14);										//clear bit 14 15 VOS
	PWR->CR      |=  (2u << 14);										//set reset value <= 84MHz
	
	/*3.configure FLASH PREFETCH and the LATENCY setting*/
	FLASH->ACR &= ~( (15u << 0)|(7u << 8) );  			//clear bit 0-3 and 8-10
	FLASH->ACR |=  ( (15u << 0)|(7u << 8) );		  	//set 5ws
	
	/*4.configure HCLK, PCLK1, PCLK2*/
	RCC->CFGR &= ~(15u << 4);												//clear bit 4-7 AHB and set AHB divide 1
	RCC->CFGR &= ~(0x3Fu << 10);										//clear bit 10-15 APB
	RCC->CFGR |=  ( (5u << 10)|(4u << 13) );				//set APB1 div 4, APB2 div 2
	
	/*5.configrate MAIN PLL*/
	RCC->PLLCFGR &= ~( (0xFFu << 0)|(0x1FFu << 6)|(3u << 16)|(1u << 22) );	//clear bit PLLM(0-5), PLLN(6-14), PLLP(16-17),PLLSCR(22)
	RCC->PLLCFGR |=  ( (4u << 0)|(10u << 6)|(0u << 16)|(1u << 22) );	      //div 4 and mult 10 and div 2
	
	/*6.enable PLL and wait ready*/
	RCC->CR &= ~(1u << 24);													//clear bit 24 25 
	RCC->CR |=  (1u << 24);													//enable PLL
	while( !((RCC->CR) & (1u << 25)) );							//wait PLL ready

	/*7.select clock soure and wait it set*/
	RCC->CFGR &= ~(3u << 0);												//clear bit 0-1
	RCC->CFGR |=  (2u << 0);												//select PLL

	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
