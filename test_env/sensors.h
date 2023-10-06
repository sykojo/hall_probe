
#pragma once
//#include "main.h"

class Sensors
{
private:
	enum SENSORS
	{
		sen_0 = 0x00,
		sen_1,
		sen_2,
		sen_3,
		sen_4,
		sen_5,
		sen_6,
		sen_7,
		sen_8
	};


	void sensorInit();

public:
	void selectSensor(int senAdr);
	void activateOE();
	void deactivateOE();
	void clockPulse();

};