#include "stdint.h"
#include "sensors.h"
#include "main.h"
#include "spi.h"
#include "spi_handler.h"


void writeToRegister(uint16_t* pRegData,REGISTER_FEATURE pos,uint16_t value)
{
        value <<= (uint16_t)pos;
        *pRegData|=value;
}


Sensor sensor_init(uint8_t adr)
{
    Sensor sen;
    sen.adr = adr;
    uint8_t crc_disable[4] = {0x0F,0x00,0x04,0x07};
    uint8_t reg_data[4] = {0};
    uint16_t realRegValue[4]={0};


    select_sensor(sen.adr);
    registers_adr_config(&sen);
    HAL_SPI_TransmitReceive(&hspi1, crc_disable, reg_data, 4, 50);
    HAL_Delay(1);
    deselect_sensor();


    device_config(&sen);
    sensor_config(&sen);
    system_config(&sen);
    realRegValue[0]=read_register(&sen, TEST_CONFIG);
    realRegValue[1]=read_register(&sen, DEVICE_CONFIG);
    realRegValue[2]=read_register(&sen, SENSOR_CONFIG);
    realRegValue[3]=read_register(&sen, SYSTEM_CONFIG);

    if(1)//registersAreSet(realRegValue))
    {
    	sen.ok = 1;
    	return sen;
    }
    else
    {
    	Sensor error_sensor;
    	error_sensor.ok = 0;
    	return error_sensor;
    }

}


void sensor_power_off()
{
	HAL_GPIO_WritePin(SEN_V_ENABLE_GPIO_Port, SEN_V_ENABLE_Pin,1);
}

void sensor_power_on()
{
	HAL_GPIO_WritePin(SEN_V_ENABLE_GPIO_Port, SEN_V_ENABLE_Pin,0);
}
void deactivateSCLR()
{
	HAL_GPIO_WritePin(SCLR_GPIO_Port, SCLR_Pin, 0);
	HAL_Delay(50);
	HAL_GPIO_WritePin(SCLR_GPIO_Port, SCLR_Pin, 1);
}



uint8_t calc_crc(REGISTER_ADR adr,uint16_t* data)
{
	uint8_t xor_result;
	uint8_t crc_polynomial=0x13;
	uint8_t crc_reg = 0x0F;
	uint8_t message[4] = {adr,(*data)>>8,*data,crc_reg};
	uint8_t crc_msb;
	uint8_t msg_msb;
	int j =0;
	for(int i=31;i>=0;i--)
	{
		crc_msb = crc_reg >> 3;
		msg_msb = (message[j]>>(i%8))&0x01;
		xor_result = crc_msb ^ msg_msb;
		crc_reg = crc_reg << 1;

		if(xor_result)
		{
			crc_reg = crc_reg ^ crc_polynomial;
		}

		if((i%8)==0){ j++; }
	}
	return crc_reg & 0x0F;
}

void registers_adr_config(Sensor* sen)
{
	sen->TEST_CONFIG.adr = TEST_CONFIG;
	sen->TEST_CONFIG.data = 0;
	sen->DEVICE_CONFIG.adr = DEVICE_CONFIG;
	sen->DEVICE_CONFIG.data = 0;
	sen->SENSOR_CONFIG.adr = SENSOR_CONFIG;
	sen->SENSOR_CONFIG.data = 0;
	sen->SYSTEM_CONFIG.adr = SYSTEM_CONFIG;
	sen->SYSTEM_CONFIG.data = 0;
	sen->CONV_STATUS.adr = CONV_STATUS;
	sen->CONV_STATUS.data = 0;
	sen->X_CH_RESULT.adr = X_CH_RESULT;
	sen->X_CH_RESULT.data = 0;
	sen->Y_CH_RESULT.adr = Y_CH_RESULT;
	sen->Y_CH_RESULT.data = 0;
	sen->Z_CH_RESULT.adr = Z_CH_RESULT;
	sen->Z_CH_RESULT.data = 0;

}

uint16_t read_register(Sensor* sen,REGISTER_ADR reg)
{
	uint16_t* pRegData=NULL;
	uint8_t regAdr = -1;

	if(reg==DEVICE_CONFIG)
	{
		pRegData = &(sen->DEVICE_CONFIG.data);
		regAdr = sen->DEVICE_CONFIG.adr;
	}
	else if(reg==SENSOR_CONFIG)
	{
		pRegData = &(sen->SENSOR_CONFIG.data);
		regAdr = sen->SENSOR_CONFIG.adr;
	}
	else if(reg==SYSTEM_CONFIG)
	{
		pRegData = &(sen->SYSTEM_CONFIG.data);
		regAdr = sen->SYSTEM_CONFIG.adr;
	}
	else if(reg==TEST_CONFIG)
	{
		pRegData = &(sen->TEST_CONFIG.data);
		regAdr = sen->TEST_CONFIG.adr;
	}
	else if(reg==CONV_STATUS)
	{
		pRegData = &(sen->CONV_STATUS.data);
		regAdr = sen->CONV_STATUS.adr;
	}
	else if(reg==X_CH_RESULT)
	{
		pRegData = &(sen->X_CH_RESULT.data);
		regAdr = sen->X_CH_RESULT.adr;
	}

	else if((pRegData==NULL)||(regAdr==-1))
	{
		return -1;
	}
	else
	{
		return -1;
	}

	select_sensor(sen->adr);
	*pRegData = spi_read_reg(regAdr, pRegData);
	deselect_sensor();
	return *pRegData;
}

uint32_t registersAreSet(uint16_t* testReg)
{
	if((*testReg==TEST_REG_EXPECTED_VAL) && (*(testReg+1) == DEVICE_CONFIG_EXPECTED_VAL) && (*(testReg+2) == SENSOR_CONFIG_EXPECTED_VAL) && (*(testReg+3) == SYSTEM_CONFIG_EXPECTED_VAL))
	{
		return 1;
	}
	else return 0;
}

void read_test_reg(Sensor* sen)
{
	select_sensor(sen->adr);
	uint16_t* pRegData= &(sen->TEST_CONFIG.data);
	*pRegData = 0;
	*pRegData = spi_read_reg(sen->TEST_CONFIG.adr, pRegData);
	if(*pRegData==0x54)
	{
		HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
	}
	deselect_sensor();
}

void device_config(Sensor* sen)
{
	//Current value: 0x502C → 0b0101 0000 0010 1100
	select_sensor(sen->adr);
	uint16_t* pRegData= &(sen->DEVICE_CONFIG.data);
	writeToRegister(pRegData, CONV_AVG, 0x05); // Best SNR
	writeToRegister(pRegData, MAG_TEMPCO,0x00); //Temperature coefficient 0%
	writeToRegister(pRegData, OPERATING_MODE, 0x2); //Continuous conversion
	writeToRegister(pRegData, T_CH_EN, 0x01);
	writeToRegister(pRegData, T_RATE, 0x01);
	writeToRegister(pRegData, T_HLT_EN, 0x00);


	spi_write_reg(sen->DEVICE_CONFIG.adr, pRegData);
	deselect_sensor();
}


void sensor_config(Sensor* sen)
{
	//Current value: 0x1055 → 0b0001 0000 0101 0101
	uint16_t* pRegData= &(sen->SENSOR_CONFIG.data);
	select_sensor(sen->adr);
	writeToRegister(pRegData,ANGLE_EN ,0x00);
	writeToRegister(pRegData,SLEEPTIME,0x04);//20ms sleeptime between conversions
	writeToRegister(pRegData,MAG_CH_EN,0x1);//Only X chanel enabled
	writeToRegister(pRegData,Z_RANGE,0x1);//+-75mT field range
	writeToRegister(pRegData,Y_RANGE,0x1);//+-75mT field range
	writeToRegister(pRegData,X_RANGE,0x1);//+-75mT field range

	spi_write_reg(sen->SENSOR_CONFIG.adr, pRegData);
	deselect_sensor();
}

void system_config(Sensor* sen)
{
	//Current value: 0x00
	uint16_t* pRegData= &(sen->SYSTEM_CONFIG.data);
	writeToRegister(pRegData, DIAG_SEL, 0x00);
	writeToRegister(pRegData, TRIGGER_MODE, 0x00); //Conversion on CS pulse
	writeToRegister(pRegData, DATA_TYPE, 0x00);
	writeToRegister(pRegData, DIAG_EN, 0x00);
	writeToRegister(pRegData, Z_HLT_EN, 0);
	writeToRegister(pRegData, Y_HLT_EN, 0);
	writeToRegister(pRegData, X_HLT_EN, 0);

	select_sensor(sen->adr);
	spi_write_reg(sen->SYSTEM_CONFIG.adr, pRegData);
	deselect_sensor();
}

void select_sensor(uint8_t adr)
{
	uint16_t message = ~(1 << (8-adr + 7));
    set_sh_reg_OE(1);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&message, 2, 10);
    set_sh_reg_OE(0);
}

void deselect_sensor()
{
	uint16_t message = 0xFFFF;
    set_sh_reg_OE(1);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&message, 2, 10);
    set_sh_reg_OE(0);
}

void set_sh_reg_OE(uint32_t value)
{
	HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, value);
}

int32_t calculate_B(int32_t data)
{
	int32_t B=0;
	int32_t sum=0;
	int32_t sign = -((data&(1<<15)?1:0)*(1<<15));
	for(int i=0;i<=14;i++)
	{
		sum += ((data&(1<<i)?1:0)*(1<<i));
	}

	B = ((sign + sum)/(1<<16))*2*75;
	return B;
}


void measuringLED()
{
	  HAL_GPIO_TogglePin(LED_MEASURING_GPIO_Port, LED_MEASURING_Pin);
	  HAL_Delay(2);
}

void errorLED()
{
	HAL_GPIO_TogglePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin);
	HAL_Delay(2);
}

/*
SensorMeasuredData read_result_reg(Sensor* sen)
{
	SensorMeasuredData* data;
	data.X_CH_DATA = &(sen->X_CH_RESULT.data);
	data.Y_CH_DATA = &(sen->Y_CH_RESULT.data);
	data.Z_CH_DATA = &(sen->Z_CH_RESULT.data);

	data.X_CH_DATA = spi_read_measured_data(sen);
	data.Y_CH_DATA;
	data.Z_CH_DATA;



}
*/

