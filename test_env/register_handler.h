#include <map>
#include <iostream>


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

    void writeToRegister(uint8_t* pReg,REGISTER_FEATURE pos,uint8_t value)
    {
        if(pReg!=nullptr)
        {
            value <<= pos;
            *pReg|=value;
        }
    }

    uint8_t* getRegisterValue(REGISTER_ADDR addr)
    {
        return &m_registerMap[addr];
    }

};

