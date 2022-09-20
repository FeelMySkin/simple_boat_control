#include "Source.h"

extern "C"
{
    void USART1_IRQHandler()
    {
        if(LL_USART_IsActiveFlag_RXNE(SBUS_USART))
        {
            sbus.AddByte(LL_USART_ReceiveData8(SBUS_USART));
        }
        if(LL_USART_IsActiveFlag_IDLE(SBUS_USART))
        {
			LL_USART_ClearFlag_IDLE(SBUS_USART);
            sbus.Parse();
        }
    }
}

int main()
{
	InitPeriph();
    InitRCC();
    InitSBUS();
    InitPWMs();

    while(1)
    {
        __WFI();
    }
}

void InitRCC()
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    LL_RCC_HSI_Enable();
    while(!LL_RCC_HSI_IsReady()) asm("NOP");
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) asm("NOP");

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,LL_RCC_PLL_MUL_12);
    LL_RCC_PLL_Enable();
	while(!LL_RCC_PLL_IsReady()) asm("NOP");
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) asm("NOP");
    SystemCoreClockUpdate();
}

void InitPeriph()
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1 | LL_APB1_GRP2_PERIPH_TIM1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);
    
    
}

void InitSBUS()
{
    SbusController_InitTypeDef sbs;
    sbs.rx_af = SBUS_AF;
    sbs.rx_gpio = SBUS_GPIO;
    sbs.rx_pin = SBUS_PIN;
    sbs.usart = SBUS_USART;
    sbus.Init(sbs);
    
}

void InitPWMs()
{
    /** Motor PWM Initialize*/
    PwmController_InitTypeDef pwm_init;
    pwm_init.pwm_af = MOTOR_AF;
    pwm_init.pwm_ch = MOTOR_TIM_CH;
    pwm_init.pwm_freq = 50;
    pwm_init.pwm_gpio = MOTOR_GPIO;
    pwm_init.pwm_pin = MOTOR_PIN;
    pwm_init.pwm_tim = MOTOR_TIM;
    motor_pwm.Init(&pwm_init);

    /** Servo PWM Initialize*/
    pwm_init.pwm_af = SERVO_AF;
    pwm_init.pwm_ch = SERVO_TIM_CH;
    pwm_init.pwm_freq = 50;
    pwm_init.pwm_gpio = SERVO_GPIO;
    pwm_init.pwm_pin = SERVO_PIN;
    pwm_init.pwm_tim = SERVO_TIM;
    servo_pwm.Init(&pwm_init);

}
