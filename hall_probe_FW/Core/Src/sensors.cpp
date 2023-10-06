
//Includes
#include "main.h"
#include "sensors.h"

//Defines
#define SENSOR_COUNT 8


enum SENSORS
{
	sen_0 = 0x01,
	sen_1 = 0x01<<1,
	sen_2 = 0x01<<2,
	sen_3 = 0x01<<3,
	sen_4 = 0x01<<4,
	sen_5 = 0x01<<5,
	sen_6 = 0x01<<6,
	sen_7 = 0x01<<7,
	sen_8 = 0x01<<8

};

void sensorInit();

int32_t selectSensor(uint16_t senAdr)
{
	bool reminder = 0;
	uint32_t bit = 0;

	while(senAdr!=0 || (bit <=(SENSOR_COUNT-1)))
	{
		reminder = senAdr%2;
		senAdr /= 2;
		if(reminder == 0 || reminder==1)
		{
			HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, reminder);
			clockPulse();
		}else{
			return -1;
		}
		bit++;
	}
	activateOE();
}

void activateOE()
{
	HAL_GPIO_WritePin(SEN_OE_GPIO_Port,SEN_OE_Pin,0);
}

void deactivateOE()
{
	HAL_GPIO_WritePin(SEN_OE_GPIO_Port,SEN_OE_Pin,1);
}

void clockPulse()
{
	HAL_GPIO_WritePin(REG_SCK_GPIO_Port, REG_SCK_Pin,1);
	HAL_Delay(50);
	HAL_GPIO_WritePin(REG_SCK_GPIO_Port, REG_SCK_Pin, 0);
}

