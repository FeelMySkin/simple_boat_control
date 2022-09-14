#include "sbus_controller.h"

void SbusController::Init(SbusController_InitTypeDef init)
{
    this->init = init;
    InitGPIO();
    InitUSART();
    recv_counter = 0;
}

void SbusController::InitGPIO()
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

void SbusController::InitUSART()
{
    LL_USART_InitTypeDef usrt;
    usrt.BaudRate = 100000;
    usrt.DataWidth = LL_USART_DATAWIDTH_8B;
    usrt.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    usrt.OverSampling = LL_USART_OVERSAMPLING_16;
    usrt.Parity = LL_USART_PARITY_EVEN;
    usrt.StopBits = LL_USART_STOPBITS_2;
    usrt.TransferDirection = LL_USART_DIRECTION_RX;
    LL_USART_Init(init.usart,&usrt);
    //LL_USART_SetBinaryDataLogic(init.usart,LL_USART_BINARY_LOGIC_NEGATIVE);
    LL_USART_SetRXPinLevel(init.usart,LL_USART_RXPIN_LEVEL_INVERTED);

    LL_USART_EnableIT_IDLE(init.usart);
    LL_USART_EnableIT_RXNE(init.usart);

    EnableUsartIrqn(init.usart,0);
    LL_USART_Enable(init.usart);
}

void SbusController::AddByte(uint8_t byte)
{
    if(recv_counter == 25) return;
    received[recv_counter] = byte;
	recv_counter++;
}

void SbusController::Parse()
{
    if(recv_counter != 25)
    {
        recv_counter = 0;
        return;
    }

    recv_counter = 0;
    
    if(received[0] == 0x0F && received[24] == 0x00)
    {
        channels[0]  = ((received[1]|received[2]<<8)                    & 0x07FF);
        channels[1]  = ((received[2]>>3 |received[3]<<5)                 & 0x07FF);
        channels[2]  = ((received[3]>>6 |received[4]<<2 |received[5]<<10)  & 0x07FF);
        channels[3]  = ((received[5]>>1 |received[6]<<7)                 & 0x07FF);
        channels[4]  = ((received[6]>>4 |received[7]<<4)                 & 0x07FF);
        channels[5]  = ((received[7]>>7 |received[8]<<1 |received[9]<<9)   & 0x07FF);
        channels[6]  = ((received[9]>>2 |received[10]<<6)                & 0x07FF);
        channels[7]  = ((received[10]>>5|received[11]<<3)                & 0x07FF);
        channels[8]  = ((received[12]   |received[13]<<8)                & 0x07FF);
        channels[9]  = ((received[13]>>3|received[14]<<5)                & 0x07FF);
        channels[10] = ((received[14]>>6|received[15]<<2|received[16]<<10) & 0x07FF);
        channels[11] = ((received[16]>>1|received[17]<<7)                & 0x07FF);
        channels[12] = ((received[17]>>4|received[18]<<4)                & 0x07FF);
        channels[13] = ((received[18]>>7|received[19]<<1|received[20]<<9)  & 0x07FF);
        channels[14] = ((received[20]>>2|received[21]<<6)                & 0x07FF);
        channels[15] = ((received[21]>>5|received[22]<<3)                & 0x07FF);
    }
}