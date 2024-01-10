#include "iostream"
#include "stdio.h"
#include <map>
#include <vector>
#include <iomanip>

#define MAX_ADDR 5

namespace REGISTERS
{
enum REGISTER_FEATURE
    {
        OPERATING_MODE=4,
        OFFSET_SELECTION=14,
        OFFSET_VALUE=7,
        GAIN_SELECTION=14,
        GAIN_VALUE=0,
        fX_CH_RESULT=0,
        fY_CH_RESULT=0,
        fZ_CH_RESULT=0

    };

    enum REGISTER_ADDR{
        DEVICE_CONFIG=0x00,
        SENSOR_CONFIG=0x11,
        MAG_OFFSET_CONFIG=0x12,
        X_CH_RESULT=0x09,
        Y_CH_RESULT=0x0A,
        Z_CH_RESULT=0x0B
    };


};

using namespace REGISTERS;

class Sensor
{
private:
    std::map<REGISTER_ADDR,uint16_t> m_registerMap;
    const uint8_t senAddr;
public:
    Sensor(uint8_t addr);
    void writeToRegister(uint16_t& pReg,REGISTER_FEATURE pos,uint8_t value);
    uint16_t& getRegisterReference(REGISTER_ADDR addr);
    const uint8_t getSenAddr() const;

};

class SPIHandler
{
private:
public:

    uint16_t spiReadRegister(uint8_t regAddr, uint16_t *stat);
    void selectSensor(uint8_t senAddr);
    void writeToConsole(uint8_t message);
    void spi_write(uint8_t regAddr,uint16_t& pReg);
};