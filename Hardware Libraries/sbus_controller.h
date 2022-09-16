/**
 * @file sbus_controller.h
 * @author your name (you@domain.com)
 * @brief SBUS receiver Controller
 * @version 0.1
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SBUS_CONTROLLER_H
#define SBUS_CONTROLLER_H

#include "defines.h"
//172~1811

/**
 * @brief Sbus Controller Initializer
 * 
 */
struct SbusController_InitTypeDef
{
    GPIO_TypeDef*   rx_gpio;
    uint32_t        rx_pin;
    uint32_t        rx_af;
    USART_TypeDef*  usart;
};


class SbusController
{
    public:
        void Init(SbusController_InitTypeDef init);
        void Parse();
        void AddByte(uint8_t byte);

        uint16_t channels[16];
        uint8_t received[25];

    private:
        void InitGPIO();
        void InitUSART();

        uint8_t recv_counter;
        SbusController_InitTypeDef init;
};

#endif