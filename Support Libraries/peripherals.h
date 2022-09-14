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
#define MOTOR_PIN       LL_GPIO_PIN_1
#define SERVO_GPIO      GPIOA
#define SERVO_PIN       LL_GPIO_PIN_2
#define SBUS_GPIO       GPIOA
#define SBUS_PIN        LL_GPIO_PIN_3
#define SBUS_REV_GPIO   GPIOA
#define SBUS_REV_PIN    LL_GPIO_PIN_4

#endif