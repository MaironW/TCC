#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "uartout.h"
#include "gps.h"

// interrupt to UART0 port, used to send data to linux terminal
void UART0IntHandler(void){
    // Clear interrupts on UART2
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)){ //loop while there are chars
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE)); //echo character
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); //blink LED
        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); //turn off LED
    }
}

// interrupt to UART1 port, used to receive GPS data to the MCU
void UART1IntHandler(void){
    char Buffer[100];
    int i=0, j=0;

    // Clear interrupts on UART2
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART1_BASE, true); //get interrupt status
    UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts

    while(UARTCharsAvail(UART1_BASE)){ //loop while there are chars
        Buffer[i] = UARTCharGetNonBlocking(UART1_BASE);
//        UARTCharPutNonBlocking(UART0_BASE, Buffer[i]); //echo character
        i++;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_1|GPIO_PIN_2); //blink LED
        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 0); //turn off LED
    }

    // parse GPS string into data array
    if(Buffer[0]=='$'){
        j=0;
        data[j] = strtok(Buffer,",");
        while(data[j] != NULL){
            data[++j] = strtok(NULL,",");
        }
    }
    UpdateDataGPS();
}


// send data through UART0
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count){
    // loop while there are more characters to send
    while(ui32Count--){
        // write the next character to the UART
        UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}

// send data through UART1
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count){
    // loop while there are more characters to send
    while(ui32Count--){
        // write the next character to the UART
        UARTCharPut(UART1_BASE, *pui8Buffer++);
    }
}

// setup for UART
void UARTsetup(uint32_t baud){

    // Enable the peripherals used:
    SysCtlPeripheralReset(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOB);
    // Port B Pins will be used as UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Port F will be used for on-board led
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Port A pins will be used as UART0(Virtual serial connection-debuse   g)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable processor interrupts.
    IntMasterEnable();

    // Set PB0 as UART1 RX
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);
    // Set PB1 as UART1 TX
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);

    // Set GPIO A1 as UART0 TX(Transmit)
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
    // Set GPIO A0 as UART0 RX
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);

    // Configure the UART0 for 115,200, 8-N-1 operation.
    // This is the serial connection between TM4C and PC
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), baud,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    // Configure the UART for 9600, 8-N-1 operation. GPS module provides data in 9600 bps
    // This is the connection between Arduino Nano
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
                     (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                      UART_CONFIG_PAR_NONE));

    // Use the internal 16MHz oscillator as the UART clock source
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

//     Initialize the UART for console I/O
    UARTStdioConfig(0, baud, SysCtlClockGet());
    UARTStdioConfig(0, baud, 16000000);

    // Enable the UART interrupts
    IntEnable(INT_UART1);
    IntEnable(INT_UART0);

    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
