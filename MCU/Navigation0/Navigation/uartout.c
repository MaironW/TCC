#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
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
//        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); //turn off LED
    }
}

// interrupt to UART2 port, used to receive GPS data to the MCU
void UART2IntHandler(void){
    // Clear interrupts on UART2
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART2_BASE, true); //get interrupt status
    UARTIntClear(UART2_BASE, ui32Status); //clear the asserted interrupts

    char Buffer[500]; // REGULAR TAMANHO
//    char GGA[80];     // string with GGA NMEA sentence
//    char VTG[80];     // string with VTG NMEA sentence

    char* temp;
    int i=0,j=0,k=0;
    while(UARTCharsAvail(UART2_BASE)){                          //loop while there are chars
        Buffer[i] = UARTCharGetNonBlocking(UART2_BASE);
//        UARTCharPutNonBlocking(UART0_BASE, Buffer[i]);        //echo character
        i++;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);  //blink LED
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));             //delay ~1 msec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);           //turn off LED
    }

    temp = strstr(Buffer,"$GPVTG"); // find VTG data inside GPS data block
        if(*temp=='$'){             // start of a line
            i=0;
            k=0;
            while(temp[j]!='\x0a'){ // wait for the last character of a line
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //blink LED
//                SysCtlDelay(SysCtlClockGet() / (10000 * 3));    //delay ~1e-10 msec
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);   //turn off LED

                if(temp[j]!=','){       // split data inside a line
                    data[i][k] = temp[j];
                    k++;
                }else{
                    data[i][k] = '\0';  // end of a line
                    i++;
                    k=0;
                }
//                    VTG[j]=temp[j];
//                UARTCharPutNonBlocking(UART0_BASE, VTG[j]); //echo character
                j++;
            }
//            UARTCharPutNonBlocking(UART0_BASE, '\x0a'); //echo character
            j=0;
        }

    temp = strstr(Buffer,"$GPGGA"); // find GGA data inside GPS data block
    if(*temp=='$'){                 // start of a line
//        i=0;
        k=0;
        while(temp[j]!='\x0a'){     // end of a line
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); //blink LED
//            SysCtlDelay(SysCtlClockGet() / (10000 * 3)); //delay ~1e-10 msec
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); //turn off LED

            if(temp[j]!=','){
                    data[i][k] = temp[j];
                k++;
            }else{
                data[i][k] = '\0';
                i++;
                k=0;
            }
//                GGA[j]=temp[j];
//            UARTCharPutNonBlocking(UART0_BASE, GGA[j]); //echo character
            j++;
        }
//        UARTCharPutNonBlocking(UART0_BASE, '\x0a'); //echo character
        j=0;
    }
}

// send data through UART0
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count){
    // loop while there are more characters to send
    while(ui32Count--){
        // write the next character to the UART
        UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}

// send data through UART2
void UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count){
    // loop while there are more characters to send
    while(ui32Count--){
        // write the next character to the UART
        UARTCharPut(UART2_BASE, *pui8Buffer++);
    }
}

// setup for UART
void UARTsetup(uint32_t baud){

    // Enable the peripherals used:
    // Port C Pins will be used as UART2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Port A pins will be used as UART0(Virtual serial connection-debuse   g)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable processor interrupts.
    IntMasterEnable();

    // Set PD6 as UART2 RX
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);
    // Set PD7 as UART2 TX
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);

    // Set GPIO A1 as UART0 TX(Transmit)
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
    // Set GPIO A0 as UART0 RX
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);

    // Configure the UART for 9600, 8-N-1 operation. GPS module provides data in 9600 bps
    // This is the connection between GPS Module(U-blox NEO6M)
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    // Use the internal 16MHz oscillator as the UART clock source
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O
//    UARTStdioConfig(0, baud, SysCtlClockGet());
    UARTStdioConfig(0, baud, 16000000);

    // Enable the UART interrupts
//    IntEnable(INT_UART2);
    IntEnable(INT_UART0);

//    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
