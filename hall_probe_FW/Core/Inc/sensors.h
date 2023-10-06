
#pragma once
#include "main.h"
#include "tmag5170.h"

class Sensors
{
private:
	SenRegisters reg;


	void senConfig();

public:
	void selectSensor(uint32_t senAdr);
	void activateOE();
	void deactivateOE();
	void clockPulse();

};


