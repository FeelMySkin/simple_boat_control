#include "pwm_controller.h"

void PwmController::Init(PwmController_InitTypeDef* init)
{
    this->init.pwm_tim = init->pwm_tim;
    this->init.pwm_ch = init->pwm_ch;
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
    tim.Autoreload = CountPrescaler(init->pwm_freq);
    LL_TIM_Init(init->pwm_tim,&tim);

    LL_TIM_OC_InitTypeDef oc;
    oc.CompareValue = 0;
    oc.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
    oc.OCMode = LL_TIM_OCMODE_PWM1;
    oc.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    oc.OCState = LL_TIM_OCSTATE_ENABLE;
    LL_TIM_OC_Init(init->pwm_tim,init->pwm_ch,&oc);
}

void PwmController::CountPrescaler(uint32_t freq)
{

}