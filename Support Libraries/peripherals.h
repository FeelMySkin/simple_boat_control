/**
 * @file peripherals.h
 * @author Phil (zetsuboulevel@gmail.com)
 * @brief Peripherals defines
 * @version 0.1
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "periph_functions.h"

/**
 * @defgroup GPIOs
 * 
 */
#define MOTOR_GPIO      GPIOA
#define MOTOR_PIN       LL_GPIO_PIN_9
#define MOTOR_AF        LL_GPIO_AF_2
#define SERVO_GPIO      GPIOA
#define SERVO_PIN       LL_GPIO_PIN_4
#define SERVO_AF        LL_GPIO_AF_4
#define CRSF_GPIO       GPIOA
#define CRSF_PIN        LL_GPIO_PIN_3
#define CRSF_AF         LL_GPIO_AF_1

/**
 * @defgroup USART
 * 
 */
#define CRSF_USART      USART1
#define CRSF_BAUD       420000

/**
 * @defgroup TIMs
 * 
 */
#define MOTOR_TIM       TIM1
#define MOTOR_TIM_CH    LL_TIM_CHANNEL_CH2
#define SERVO_TIM       TIM14
#define SERVO_TIM_CH    LL_TIM_CHANNEL_CH1

#define CRSF_TIM		TIM16

#endif