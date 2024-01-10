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
    Sensor(uint8_t addr) : m_registerMap(), senAddr(addr)
    {
        uint8_t startAddr=REGISTER_ADDR::DEVICE_CONFIG;
        uint8_t endAddr=REGISTER_ADDR::Z_CH_RESULT;
        //Initialize all registers to 0x00
        for(uint8_t addr=startAddr; addr<endAddr;++addr)
        {
            m_registerMap[static_cast<REGISTER_ADDR>(addr)] = 0x00;
        }

        //Initialize registers according to datasheet
            //Set OPERATING_MODE to Continuous conversion
                writeToRegister(m_registerMap[DEVICE_CONFIG],OPERATING_MODE,0x02);
            
    }

    void writeToRegister(uint16_t& pReg,REGISTER_FEATURE pos,uint8_t value)
    {
            value <<= pos;
            pReg|=value;
    }

    uint16_t& getRegisterReference(REGISTER_ADDR addr)
    {
        return m_registerMap[addr];
    }

    const uint8_t getSenAddr() const
    {
        return this->senAddr;
    }

};

class SPIHandler
{
private:
public:

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
    void selectSensor(uint8_t senAddr)
    {
        //if(){
            //HAL_SPI_TransmitIT(&hspi2,message,1,50);
        //}
    }

    void writeToConsole(uint8_t message)
    {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(message) <<std::endl;
    }


    void spi_write(uint8_t regAddr,uint16_t& pReg)
    {//!!!!!!!!!!!!!!!! IF PRVNÍ BIT ==0
        uint8_t message[4];
        regAddr |= 0x00;              //First MSB bitm must be 
        message[0] = regAddr;         //Write an address with first bit set to 0
        message[1] = pReg>>8;    //Write data to register 8MSB bits
        message[2] = pReg;       //Write data to register 8LSB bits
        message[3] = 0x00; //TODO: CRC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!        //Write cmd to start conversion on next CS going high
        std::cin.get();
//      HAL_SPI_Transmit(&hspi1,message,4);
    }
};


int main()
{
    
    SPIHandler spi1;
    std::vector<Sensor> sensors;
    /*
    Sensor sen(0x01);
    spi1.spi_write(DEVICE_CONFIG,sen.getRegisterReference(DEVICE_CONFIG));
    spi1.spiReadRegister(SENSOR_CONFIG);
    */

    //_______________________USEFULL CODE__________________________________

    for(uint8_t addr=0x00;addr<MAX_ADDR;addr++)
    {
        Sensor sen(addr);
        sensors.push_back(sen);
    }

    for(Sensor sen : sensors)
    {
        uint16_t dataBuff;
        uint8_t senAddr = sen.getSenAddr();
        uint16_t& xChResultReg = sen.getRegisterReference(REGISTER_ADDR::X_CH_RESULT);
        REGISTER_FEATURE feature = REGISTER_FEATURE::fX_CH_RESULT;
            
    //SPI commands to get the data from the register
        spi1.selectSensor(senAddr);
        //dataBuff = spi1.spiReadRegister(senAddr);

    //Save the value of the register
        sen.writeToRegister(xChResultReg,feature,dataBuff);
    }

}