/**
 * @file Source.h
 * @author Phil (zetsuboulevel@gmail.com)
 * @brief Source file
 * @version 0.1
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SOURCE_H
#define SOURCE_H

#include "defines.h"
#include "crsf.h"
#include "pwm_controller.h"

CRSF_Controller crsf;
PwmController motor_pwm, servo_pwm;

bool armed;


void InitRCC();
void InitPeriph();
void InitCRSF();
void InitPWMs();

#endif