// TIVA C Library for UART print in terminal without using UARTprintf function
// written by: Mairon S. Wolniewicz
// last update: 25/04/2019


void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count);

// setup for UART
void UARTsetup(uint32_t baud);
