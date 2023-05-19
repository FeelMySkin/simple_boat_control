#include "Source.h"

extern "C"
{
    void USART1_IRQHandler()
    {
		if(LL_USART_IsActiveFlag_ORE(CRSF_USART)) LL_USART_ClearFlag_ORE(CRSF_USART);
        if(LL_USART_IsActiveFlag_RXNE(CRSF_USART))
        {
            crsf.AddByte(LL_USART_ReceiveData8(CRSF_USART));
        }
        /*if(LL_USART_IsActiveFlag_IDLE(CRSF_USART))
        {
			LL_USART_ClearFlag_IDLE(CRSF_USART);
            
        }*/
    }

	void TIM16_IRQHandler()
	{
		LL_TIM_ClearFlag_UPDATE(CRSF_TIM);
		crsf.Parse();
	}
}

#define SERVO_CH    0
#define MOTOR_CH    2
#define ARM_CH      4
float test_duty = 0;

int main()
{
	InitPeriph();
    InitRCC();
    InitCRSF();
    InitPWMs();

    armed = false;

	//while(1) asm("NOP");
    while(1)
    {
        servo_pwm.SetDuty(Map<float>(crsf.mapped_channels[SERVO_CH],0.f,2048.f,MIN_PERCENT,MAX_PERCENT));
        //motor_pwm.SetDuty(test_duty);
        //servo_pwm.SetDuty((sbus.mapped_channels[SERVO_CH]/2048.0f)*100.0f);
        if(crsf.mapped_channels[ARM_CH]<=500 || crsf.stat.uplinkLQ <=5)
        {
            motor_pwm.SetDuty(MIN_MOTOR_PERCENT);
            armed = false;
        }
        else if(!armed)
        {
            motor_pwm.SetDuty(MIN_MOTOR_PERCENT);
            if(crsf.mapped_channels[MOTOR_CH]<=10) armed = true;
        }
        else if(armed)
        {
            motor_pwm.SetDuty(Map<float>(crsf.mapped_channels[MOTOR_CH],0.f,2048.f,MIN_MOTOR_PERCENT,MAX_MOTOR_PERCENT));
        }
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
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1 | LL_APB1_GRP2_PERIPH_TIM1 | LL_APB1_GRP2_PERIPH_TIM16);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);
    
    
    
}

void InitCRSF()
{
    CRSF_TypeDef sbs;
    sbs.rx_af = CRSF_AF;
    sbs.rx_gpio = CRSF_GPIO;
    sbs.rx_pin = CRSF_PIN;
    sbs.usart = CRSF_USART;
	sbs.control_tim = CRSF_TIM;
    crsf.Init(sbs);
    
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
    motor_pwm.SetDuty(5);

    /** Servo PWM Initialize*/
    pwm_init.pwm_af = SERVO_AF;
    pwm_init.pwm_ch = SERVO_TIM_CH;
    pwm_init.pwm_freq = 50;
    pwm_init.pwm_gpio = SERVO_GPIO;
    pwm_init.pwm_pin = SERVO_PIN;
    pwm_init.pwm_tim = SERVO_TIM;
    servo_pwm.Init(&pwm_init);
    servo_pwm.SetDuty(7.5f);
}

template <class T>
T Map(T input, T min_input, T max_input, T min_output, T max_output)
{
    T output = ((input-min_input)/(max_input-min_input))*(max_output-min_output) + min_output;
    return output;
}