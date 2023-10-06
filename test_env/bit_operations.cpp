#include "iostream"
#include "stdio.h"
#include <map>


enum REGISTER_FEATURE
{
    OFFSET_SELECTION=14,
    OFFSET_VALUE=7,
    GAIN_SELECTION=14,
    GAIN_VALUE=0
};

enum REGISTER_ADDR{
    DEVICE_CONFIG=0x00,
    SENSOR_CONFIG=0x11,
    MAG_OFFSET_CONFIG=0x12
};

class RegisterHandler
{
private:
    std::map<REGISTER_ADDR,uint8_t> m_registerMap;
    REGISTER_ADDR m_regAddr;
public:
    RegisterHandler()
    {
        for(auto &kv : m_registerMap)
        {
           kv.second = 0x00;
        }
    }

    void modifyRegister(uint8_t* pReg,REGISTER_FEATURE pos,uint8_t value)
    {
        if(pReg!=nullptr)
        {
            value <<= pos;
            *pReg|=value;
        }
    }

    uint8_t* getRegister(REGISTER_ADDR addr)
    {
        return &m_registerMap[addr];
    }

};



class SPIHandler
{
private:
public:

    void spi_write(REGISTER_ADDR addr, uint8_t* pReg)
    {
        uint8_t message[4]={addr,*pReg};

//        HAL_SPI_Transmit(&hspi1,message,4);
    }
};



int main()
{

SPIHandler spi1;
RegisterHandler r;

//r.modifyRegister(r.getMAG_OFFSET_CONFIG(),OFFSET_SELECTION,0x01);
spi1.spi_write(MAG_OFFSET_CONFIG,r.getRegister(MAG_OFFSET_CONFIG));
std::cin.get();
}