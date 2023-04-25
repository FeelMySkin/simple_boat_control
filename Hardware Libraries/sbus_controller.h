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
#define MAX_FRAME_SIZE	64

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
        uint16_t mapped_channels[16];

    private:
        void InitGPIO();
        void InitUSART();
        void Map();

        uint8_t recv_counter;
        uint16_t channels[16];
        uint8_t received[MAX_FRAME_SIZE];
        SbusController_InitTypeDef init;
};

#endif