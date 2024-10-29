#include "RCC.h"
#include "DL.h"
#include "I2C.h"
#include "LCD.h"
#include "RPM.h"
#include "stdio.h"

void TIM3_Init(void);
void TIM3_IRQHandler(void);

volatile uint32_t encoder_count = 0;
#define PPR 20

static unsigned long  rpm = 0.0;
static unsigned long count = 0.0;

static char buffer[16];

int main ()
{
	SysClockConfig ();
	TIM9Config ();
	I2C_Config ();	
	
	GPIO_Init();
  EXTI_Init();
	TIM3_Init();
	
	lcd_init ();

	while (1)
	{
		
		lcd_put_cur (0,0);
		snprintf(buffer, sizeof(buffer), "Count: %lu", rpm );
		lcd_send_string(buffer);
		//encoder_count = 0;
		Delay_ms(100);  // Update display every 500ms
		
	}
}

void TIM3_Init(void) 
{
    // Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Configure TIM3
    TIM3->PSC = 10 - 1;  // Prescaler = 8400 (t?n s? d?m 10 kHz)
    TIM3->ARR = 1000 - 1;  // Auto-reload m?i 100ms
    TIM3->DIER |= TIM_DIER_UIE;  // Cho phép ng?t update
    TIM3->CR1 |= TIM_CR1_CEN;    // B?t Timer 3
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF) {  // Ki?m tra ng?t Timer
        // Tính RPM m?i 100ms
			count++;
        rpm = (encoder_count * 600) / PPR;  // 600 = (60 giây / 0.1 giây)
        encoder_count = 0;  // Reset s? lu?ng xung sau m?i 100ms
        TIM3->SR &= ~TIM_SR_UIF;  // Clear c? ng?t Timer
    }
}
