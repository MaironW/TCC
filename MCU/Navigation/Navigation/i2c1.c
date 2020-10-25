#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "i2c1.h"

//initialize I2C module 1
//Slightly modified version of TI's example code
void InitI2C1(void){
    //enable I2C module 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

    //enable GPIO peripheral that contains I2C 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the pin muxing for I2C1 functions on port A6 and A7.
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    // Enable and initialize the I2C1 master module.  Use the system clock for
    // the I2C1 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;
}

void WriteI2C1(uint8_t device_address, uint8_t device_register, uint8_t device_data){
   //specify that we want to communicate to device address with an intended write to bus
   I2CMasterSlaveAddrSet(I2C1_BASE, device_address, false);

   //register to be read
   I2CMasterDataPut(I2C1_BASE, device_register);

   //send control byte and register address byte to slave device
   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

   //wait for MCU to finish transaction
   while(I2CMasterBusy(I2C1_BASE));

   //specify data to be written to the above mentioned device_register
   I2CMasterDataPut(I2C1_BASE, device_data);

   //wait while checking for MCU to complete the transaction
   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

   //wait for MCU & device to complete transaction
   while(I2CMasterBusy(I2C1_BASE));
}

uint16_t ReadI2C1(uint8_t device_address, uint8_t device_register){
   //specify that we want to communicate to device address with an intended write to bus
   I2CMasterSlaveAddrSet(I2C1_BASE, device_address, false);

   //the register to be read
   I2CMasterDataPut(I2C1_BASE, device_register);

   //send control byte and register address byte to slave device
   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

   //wait for MCU to complete send transaction
   while(I2CMasterBusy(I2C1_BASE));

   //read from the specified slave device
   I2CMasterSlaveAddrSet(I2C1_BASE, device_address, true);

   //send control byte and read from the register from the MCU
   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

   //wait while checking for MCU to complete the transaction
   while(I2CMasterBusy(I2C1_BASE));

   //Get the data from the MCU register and return to caller
   return(I2CMasterDataGet(I2C1_BASE));
 }
