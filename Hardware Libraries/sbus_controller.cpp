#include "sbus_controller.h"

void SbusController::Init(SbusController_InitTypeDef init)
{
    this->init = init;
    InitGPIO();
    InitUSART();
    recv_counter = 0;
}

struct crsf_channels_s
{
	unsigned ch0 : 11;
	unsigned ch1 : 11;
	unsigned ch2 : 11;
	unsigned ch3 : 11;
	unsigned ch4 : 11;
	unsigned ch5 : 11;
	unsigned ch6 : 11;
	unsigned ch7 : 11;
	unsigned ch8 : 11;
	unsigned ch9 : 11;
	unsigned ch10 : 11;
	unsigned ch11 : 11;
	unsigned ch12 : 11;
	unsigned ch13 : 11;
	unsigned ch14 : 11;
	unsigned ch15 : 11;
};

union chals
{
	uint8_t rcv[22];
	crsf_channels_s channels;
}true_ch;

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
    LL_USART_EnableIT_RXNE(init.usart);

    EnableUsartIrqn(init.usart,0);
    LL_USART_Enable(init.usart);
}

void SbusController::AddByte(uint8_t byte)
{
    if(recv_counter == MAX_FRAME_SIZE) return;
    received[recv_counter] = byte;
	recv_counter++;
}

void SbusController::Parse()
{
/*    if(recv_counter != MAX_FRAME_SIZE)
    {
        recv_counter = 0;
        return;
    }*/
    
    if(received[recv_counter-1] == 0xC8)
    {
		for(int i = 0;i<22;++i) true_ch.rcv[i] = received[3+i];
        channels[0]  = ((received[3]|received[4]<<8)                    & 0x07FF);
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
        channels[15] = ((received[23]>>5|received[24]<<3)                & 0x07FF);
    }
	
    recv_counter = 0;
    Map();
}

void SbusController::Map()
{
    //min = 172, max = 1811
    for(int i = 0;i<16;++i)
    {
        mapped_channels[i] = ((channels[i]-172)/(1811-172.0f))*2047;
    }
}