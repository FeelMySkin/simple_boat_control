/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "crsf.h"

/*#include "platform.h"

#include "build/build_config.h"
#include "build/debug.h"

#include "common/crc.h"
#include "common/maths.h"
#include "common/utils.h"

#include "drivers/time.h"
#include "drivers/serial.h"
#include "drivers/serial_uart.h"

#include "io/serial.h"
#include "io/osd.h"

#include "rx/rx.h"
#include "rx/crsf.h"

#include "telemetry/crsf.h"*/
#define CRSF_TIME_NEEDED_PER_FRAME_US   1100 // 700 ms + 400 ms for potential ad-hoc request
#define CRSF_TIME_BETWEEN_FRAMES_US     6667 // At fastest, frames are sent by the transmitter every 6.667 milliseconds, 150 Hz

#define CRSF_DIGITAL_CHANNEL_MIN 172
#define CRSF_DIGITAL_CHANNEL_MAX 1811
#define CRSF_PAYLOAD_OFFSET offsetof(crsfFrameDef_t, type)
#define CRSF_POWER_COUNT 9

#include "sbus_controller.h"

void CRSF_Controller::Init(CRSF_TypeDef init)
{
    this->init = init;
    InitGPIO();
    InitUSART();
	InitTIM();
    recv_counter = 0;
}

void CRSF_Controller::InitTIM()
{
	LL_TIM_InitTypeDef tim;
	tim.Autoreload = 500;
	tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	tim.CounterMode = LL_TIM_COUNTERMODE_UP;
	tim.Prescaler = SystemCoreClock/1000000;
	tim.RepetitionCounter = 0;
	LL_TIM_Init(init.control_tim,&tim);

	LL_TIM_SetOnePulseMode(init.control_tim,LL_TIM_ONEPULSEMODE_SINGLE);

	LL_TIM_EnableIT_UPDATE(init.control_tim);
	EnableTimIRQn(init.control_tim,2);
}

void CRSF_Controller::InitGPIO()
{
    LL_GPIO_InitTypeDef gpio;
    gpio.Alternate = init.rx_af;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Pin = init.rx_pin;
    gpio.Pull = LL_GPIO_PULL_NO;
    gpio.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(init.rx_gpio, &gpio);
}

void CRSF_Controller::InitUSART()
{
    LL_USART_InitTypeDef usrt;
    usrt.BaudRate = 420000;
    usrt.DataWidth = LL_USART_DATAWIDTH_8B;
    usrt.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    usrt.OverSampling = LL_USART_OVERSAMPLING_16;
    usrt.Parity = LL_USART_PARITY_NONE;
    usrt.StopBits = LL_USART_STOPBITS_1;
    usrt.TransferDirection = LL_USART_DIRECTION_RX;
    LL_USART_Init(init.usart,&usrt);
	
    //LL_USART_SetBinaryDataLogic(init.usart,LL_USART_BINARY_LOGIC_NEGATIVE);
    LL_USART_SetRXPinLevel(init.usart,LL_USART_RXPIN_LEVEL_STANDARD);

    LL_USART_EnableIT_IDLE(init.usart);
    //LL_USART_EnableIT_RXNE(init.usart);

    EnableUsartIrqn(init.usart,0);
    LL_USART_Enable(init.usart);
}

void CRSF_Controller::AddByte(uint8_t byte)
{
    if(recv_counter == MAX_FRAME_SIZE) return;
	LL_TIM_SetCounter(init.control_tim, 0);
	if(recv_counter == 0) LL_TIM_EnableCounter(init.control_tim);
    received[recv_counter] = byte;
	recv_counter++;
}

void CRSF_Controller::Parse()
{
/*    if(recv_counter != MAX_FRAME_SIZE)
    {
        recv_counter = 0;
        return;
    }*/
    
    if(received[recv_counter-1] == 0xC8 && received[0] == CRSF_ADDRESS_FLIGHT_CONTROLLER)
    {
		if(received[1] == recv_counter-2 && received[2] == CRSF_FRAMETYPE_RC_CHANNELS_PACKED)
			for(int i = 0;i<CRSF_FRAME_RC_CHANNELS_PAYLOAD_SIZE;++i) true_ch.rcv[i] = received[3+i];
        /*channels[0]  = ((received[3]|received[4]<<8)                    & 0x07FF);
        channels[1]  = ((received[4]>>3 |received[5]<<5)                 & 0x07FF);
        channels[2]  = ((received[5]>>6 |received[6]<<2 |received[7]<<10)  & 0x07FF);
        channels[3]  = ((received[7]>>1 |received[8]<<7)                 & 0x07FF);
        channels[4]  = ((received[8]>>4 |received[9]<<4)                 & 0x07FF);
        channels[5]  = ((received[9]>>7 |received[10]<<1 |received[11]<<9)   & 0x07FF);
        channels[6]  = ((received[11]>>2 |received[12]<<6)                & 0x07FF);
        channels[7]  = ((received[12]>>5|received[13]<<3)                & 0x07FF);
        channels[8]  = ((received[14]   |received[15]<<8)                & 0x07FF);
        channels[9]  = ((received[15]>>3|received[16]<<5)                & 0x07FF);
        channels[10] = ((received[16]>>6|received[17]<<2|received[18]<<10) & 0x07FF);
        channels[11] = ((received[18]>>1|received[19]<<7)                & 0x07FF);
        channels[12] = ((received[19]>>4|received[20]<<4)                & 0x07FF);
        channels[13] = ((received[20]>>7|received[21]<<1|received[22]<<9)  & 0x07FF);
        channels[14] = ((received[22]>>2|received[23]<<6)                & 0x07FF);
        channels[15] = ((received[23]>>5|received[24]<<3)                & 0x07FF);*/
    }
	
    recv_counter = 0;
    Map();
}

void CRSF_Controller::Map()
{
    //min = 172, max = 1811
    for(int i = 0;i<16;++i)
    {
        mapped_channels[i] = ((channels[i]-172)/(1811-172.0f))*2047;
    }
}
