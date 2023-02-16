/**
 * @file defines.h
 * @author Phil (zetsuboulevel@gmail.com)
 * @brief Some Libraries includes and periph defines defines
 * @version 0.1
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DEFINES_H
#define DEFINES_H

#include "peripherals.h"
#include "stdint.h"

#define MIN_PERCENT     5
#define MAX_PERCENT     10

template <class T>
T Map(T input, T min_input, T max_input, T min_output, T max_output);

#endif
