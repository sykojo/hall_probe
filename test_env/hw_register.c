#include "stdio.h"
#include "stdint.h"

typedef enum 
    {
        OPERATING_MODE=4,
        OFFSET_SELECTION=14,
        OFFSET_VALUE=7,
        GAIN_SELECTION=14,
        GAIN_VALUE=0,
        fX_CH_RESULT=0,
        fY_CH_RESULT=0,
        fZ_CH_RESULT=0

    }REGISTER_FEATURE;

    typedef enum{
        DEVICE_CONFIG=0x00,
        SENSOR_CONFIG=0x11,
        MAG_OFFSET_CONFIG=0x12,
        X_CH_RESULT=0x09,
        Y_CH_RESULT=0x0A,
        Z_CH_RESULT=0x0B
    }REGISTER_ADR;

typedef struct
{
    uint32_t adr;
    uint16_t data;
}Register;

typedef struct
{
    uint32_t adr;
    Register DEVICE_CONFIG;
    Register SENSOR_CONFIG;
    Register MAG_OFFSET_CONFIG;
    Register X_CH_RESULT;
    Register Y_CH_RESULT;
    Register Z_CH_RESULT;
}Sensor;

    uint16_t spiReadRegister(uint8_t regAddr, uint16_t *stat)
    {
        uint8_t txData[4]={0x00};
        uint8_t rxData[4]={0x00};
        regAddr |= 0x80;                       
        txData[0] = regAddr; 
        //HAL_SPI_TransmitRecieve(&hspi1,txData,rxData,4,50);
        *stat = (rxData[0]<<8) + rxData[3];
        return ((rxData[1]<<8) + rxData[2]);
    }

    void spi_write(uint8_t regAdr,uint16_t* pRegData)
    {
        uint8_t message[4];
        regAdr |= 0x00;              //First MSB bitm must be 
        message[0] = regAdr;         //Write an address with first bit set to 0
        message[1] = *pRegData>>8;    //Write data to register 8MSB bits
        message[2] = *pRegData;       //Write data to register 8LSB bits
        message[3] = 0x00; //TODO: CRC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!        //Write cmd to start conversion on next CS going high

//      HAL_SPI_Transmit(&hspi1,message,4);
    }

    void writeToRegister(uint16_t* pRegBits,REGISTER_FEATURE pos,uint8_t value)
    {
            value <<= pos;
            *pRegBits|=value;
    }    

    Sensor sensor_init(uint32_t adr)
    {
        Sensor sen;
        sen.adr = adr;
        sen.DEVICE_CONFIG.adr = DEVICE_CONFIG;
        sen.SENSOR_CONFIG.adr = SENSOR_CONFIG;
        return sen;
    }


int main()
{
    Sensor sen1;
    sen1 = sensor_init(5);

    Sensor sensors[5];

    for(int senAdr=0;senAdr<5;senAdr++)
    {
        sensors[senAdr] = sensor_init(senAdr);
    }

    writeToRegister(&sensors[2].SENSOR_CONFIG.data,OPERATING_MODE,0x02);

}