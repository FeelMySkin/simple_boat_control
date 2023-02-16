#include "pwm_controller.h"

void PwmController::Init(PwmController_InitTypeDef* init)
{
    this->init.pwm_tim = init->pwm_tim;
    this->init.pwm_ch = init->pwm_ch;
    this->init.desired_resolution = 4000;
    CountResolution(init->pwm_freq);
    InitGPIO(init);
    InitTIM(init);
}

void PwmController::InitGPIO(PwmController_InitTypeDef* init)
{
    LL_GPIO_InitTypeDef gpio;
    gpio.Alternate = init->pwm_af;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Pin = init->pwm_pin;
    gpio.Pull = LL_GPIO_PULL_NO;
    gpio.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(init->pwm_gpio,&gpio);
}

void PwmController::InitTIM(PwmController_InitTypeDef* init)
{
    LL_TIM_InitTypeDef tim;
    tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    tim.RepetitionCounter = 0;
    tim.Prescaler = CountPrescaler(init->pwm_freq);
    tim.Autoreload = this->init.desired_resolution;
    LL_TIM_Init(init->pwm_tim,&tim);

    LL_TIM_OC_InitTypeDef oc;
    oc.CompareValue = 0;
    oc.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
    oc.OCMode = LL_TIM_OCMODE_PWM1;
    oc.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    oc.OCState = LL_TIM_OCSTATE_ENABLE;
    LL_TIM_OC_Init(init->pwm_tim,init->pwm_ch,&oc);
	
	LL_TIM_EnableCounter(init->pwm_tim);
	LL_TIM_CC_EnableChannel(init->pwm_tim,init->pwm_ch);
	LL_TIM_EnableAllOutputs(init->pwm_tim);
}

uint32_t PwmController::CountPrescaler(uint32_t freq)
{
    
    return SystemCoreClock/(init.desired_resolution*freq) - 1;
}

void PwmController::CountResolution(uint32_t freq)
{
    while(SystemCoreClock%init.desired_resolution != 0)
    {
        init.desired_resolution--;
    }
    while((SystemCoreClock/init.desired_resolution)/freq >=0xFFFF)
    {
        init.desired_resolution*=2;
    }
    
}

void PwmController::SetDuty(float percent)
{
    if(percent>100) percent = 100;
    curr_percent = percent;
    uint32_t res = percent*LL_TIM_GetAutoReload(init.pwm_tim)/100.0f;
    if(init.pwm_ch == LL_TIM_CHANNEL_CH1) LL_TIM_OC_SetCompareCH1(init.pwm_tim,res);
    else if(init.pwm_ch == LL_TIM_CHANNEL_CH2) LL_TIM_OC_SetCompareCH2(init.pwm_tim,res);
    else if(init.pwm_ch == LL_TIM_CHANNEL_CH3) LL_TIM_OC_SetCompareCH3(init.pwm_tim,res);
    else if(init.pwm_ch == LL_TIM_CHANNEL_CH4) LL_TIM_OC_SetCompareCH4(init.pwm_tim,res);
}