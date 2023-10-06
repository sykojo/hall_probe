#include <iostream>
#include "sensors.h"

#define WORD_LENGTH 8 

/*
int32_t selectSensor(uint32_t senAdr)
{
	bool reminder = 0;
	uint32_t bit = 0;
	
	while((senAdr!=0) || (bit <= (WORD_LENGTH-1)) )
    {
		reminder = senAdr%2;
		senAdr = senAdr/2;
		if(reminder == 0 || reminder==1)
		{
		    std::cout<< reminder << std::endl;
		}else{
			return -1;
		}
        bit++;
	}
    return 0;
}
*/
int main()
{
/*    uint32_t senAdr = (0x01<<8);
    uint32_t result = selectSensor(senAdr);
	*/

	Sensors sensors = Sensors();

	sensors.clockPulse();
}