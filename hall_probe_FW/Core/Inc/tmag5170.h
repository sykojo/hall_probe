struct SenRegisters{
	uint8_t OPERATING_MODE; //0H Config, 2h Active Measure, 3h Active trigger
	uint8_t TRIGGER_MODE; 	//0H SPI, 1H CS, 2H ALERT,3H NOT USED
	uint_8_t DATA_TYPE;
	uint8_t RDY;			//OH NOT VALID, 1H VALID


};
