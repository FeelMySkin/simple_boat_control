/**
 * @file pwm_controller.h
 * @author your name (you@domain.com)
 * @brief PWM  Controller Class
 * @version 0.1
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include "defines.h"

struct PwmController_PrivateInit
{
    TIM_TypeDef*    pwm_tim;
    uint32_t        pwm_ch;
};

struct PwmController_InitTypeDef : public PwmController_PrivateInit
{
    //TIM_TypeDef*    pwm_tim;
    GPIO_TypeDef*   pwm_gpio;
    uint32_t        pwm_pin;
    uint32_t        pwm_af;
    uint32_t        pwm_freq;
    uint32_t        resolution;
    //uint32_t        pwm_ch;
};

class PwmController
{
    public:
        void Init(PwmController_InitTypeDef* init);
        void SetDuty(uint8_t percent);

    private:
        void InitGPIO(PwmController_InitTypeDef* init);
        void InitTIM(PwmController_InitTypeDef* init);
        void CountPrescaler(uint32_t freq);
        void CountAutoreload(uint32_t freq);

        PwmController_PrivateInit init;
};

#endif
