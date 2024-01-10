#ifndef SENSORS
#define SENSORS

#include "stdio.h"
#include "stdint.h"

typedef enum
    {
		//Test config
		CRC_DIS=2,
		//Device config
		CONV_AVG=14,
		MAG_TEMPCO=9,
		OPERATING_MODE=6,
		T_CH_EN=3,
		T_RATE=2,
		T_HLT_EN=1,
		//System config
		DIAG_SEL=13,
		TRIGGER_MODE=10,
		DATA_TYPE=8,
		DIAG_EN=5,
		Z_HLT_EN=2,
		Y_HLT_EN=1,
		X_HLT_EN=0,
		// Temperature result
		TEMP_RESULT_SH=15



    }REGISTER_FEATURE;

    typedef enum
    {
        DEVICE_CONFIG=0x00,
        SENSOR_CONFIG=0x01,
		SYSTEM_CONFIG=0x02,
		ALERT_CONFIG=0x03,
		X_THRX_CONFIG=0x04,
		Y_THRX_CONFIG=0x05,
		Z_THRX_CONFIG=0x06,
		T_THRX_CONFIG=0x07,
		CONV_STATUS=0x08,
		X_CH_RESULT=0x09,
		Y_CH_RESULT=0x0A,
		Z_CH_RESULT=0x0B,
		TEMP_RESULT=0x0C,
		AFE_STATUS=0x0D,
		SYS_STATUS=0x0E,
		TEST_CONFIG=0x0F,
		OSC_MONITOR=0x10,
		MAG_GAIN_CONFIG=0x11,
		MAG_OFFSET_CONFIG=0x12,
		ANGLE_RESULT=0x13,
		MAGNITUDE_RESULT=0x14
    }REGISTER_ADR;

typedef struct
{
    REGISTER_ADR adr;
    uint16_t data;
}Register;

typedef struct
{
    uint8_t adr;
    Register DEVICE_CONFIG;
    Register SENSOR_CONFIG;
    Register SYSTEM_CONFIG;
    Register ALERT_CONFIG;
    Register X_THRX_CONFIG;
    Register Y_THRX_CONFIG;
    Register Z_THRX_CONFIG;
    Register T_THRX_CONFIG;
    Register CONV_STATUS;
    Register X_CH_RESULT;
    Register Y_CH_RESULT;
    Register Z_CH_RESULT;
    Register TEMP_RESULT;
    Register AFE_STATUS;
    Register SYS_STATUS;
    Register TEST_CONFIG;
	Register OSC_MONITOR;
	Register MAG_GAIN_CONFIG;
	Register MAG_OFFSET_CONFIG;
	Register ANGLE_RESULT;
	Register MAGNITUDE_RESULT;
}Sensor;

void writeToRegister(uint16_t* pRegData,REGISTER_FEATURE pos,uint8_t value);
Sensor sensor_init(uint32_t adr);
void sensor_power_on();
void sensor_power_off();
void registers_adr_config(Sensor* sen);
void test_config(Sensor* sen);
void device_config(Sensor* sen);
void system_config(Sensor* sen);
void select_sensor(uint8_t adr);
void clear_sh_reg();
void set_sh_reg_output(uint32_t value);
uint8_t calc_crc(REGISTER_ADR adr,uint16_t* data);

#endif


