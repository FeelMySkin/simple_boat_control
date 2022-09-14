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
#define SBUS_GPIO       GPIOA
#define SBUS_PIN        LL_GPIO_PIN_3
#define SBUS_AF         LL_GPIO_AF_1

/**
 * @defgroup USART
 * 
 */
#define SBUS_USART      USART1
#define SBUS_BAUD       100000

/**
 * @defgroup TIMs
 * 
 */
#define MOTOR_TIM       TIM1
#define SERVO_TIM       TIM14

#endif