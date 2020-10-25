//initialize I2C module 1
//Slightly modified version of TI's example code
void InitI2C1(void);

//sends an I2C command to the specified slave
void WriteI2C1(uint8_t device_address, uint8_t device_register, uint8_t device_data);

//read specified register on slave device
uint16_t ReadI2C1(uint8_t device_address, uint8_t device_register);
