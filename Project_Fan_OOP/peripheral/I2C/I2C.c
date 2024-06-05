﻿#include "I2C.h"

void I2C_init(){
	I2C_DDR |= (1<<I2C_SCL)|(1<<I2C_SDA); // write Mode 사용
	TWBR = 72; // 100kHz SCL (clock 설정)
	TWSR = 0;
}

void I2C_txByte(uint8_t SLA_W, uint8_t data){
	//1.start bit
	I2C_start();
	//2.device address + writeMode + ACK
	I2C_txDevAddress(SLA_W);
	//3.data TX + ACK
	I2C_txData(data);
	//4.stop bit
	I2C_stop();
}

void I2C_start(){
	//Send START condition
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	//Wait for TWINT flag set. 
	//This indicates that the START condition has been transmitted
	while (!(TWCR & (1<<TWINT)));
}

void I2C_txDevAddress(uint8_t SLA_W){
	//Load SLA_W into TWDR Register. 
	//Clear TWINT bit in TWCR to start transmission of address
	TWDR = SLA_W;
	TWCR = (1<<TWINT)|(1<<TWEN);
	//Wait for TWINT flag set. 
	//This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
	while (!(TWCR & (1<<TWINT)));
}

void I2C_txData(uint8_t data){
	//Load DATA into TWDR Register. 
	//Clear TWINT bit in TWCR to start transmission of data
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	//Wait for TWINT flag set. 
	//This indicates that the DATA has been transmitted, and ACK/NACK has been received.
	while (!(TWCR & (1<<TWINT)));
}

void I2C_stop(){
	//Transmit STOP condition
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}