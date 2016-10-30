#include "serial.h"

void init_serial(){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
  ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
  ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(),115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  IntEnable(INT_UART1);
  UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
  UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
  UARTFIFOEnable(UART1_BASE);
}

void UARTIntHandler(void){

  unsigned long ulStatus;
  ulStatus = UARTIntStatus(UART1_BASE, true);
  UARTIntClear(UART1_BASE, ulStatus);

  char message[MAX_MESSAGE_LEN];
  int received=0;

  while(UARTCharsAvail(UART1_BASE) && received<MAX_MESSAGE_LEN){
	  *(message+received)=UARTCharGet(UART1_BASE);
	  received++;

  }

  if(received == MAX_MESSAGE_LEN){
	  switch(message[0]){
  	  	  case 'd':
  	  		  drive_motor(message[1]);
  	  	  break;
  	  	  case 's':
  	  		  motor_set_speed(message[1]);
  	  	  break;
	  }
  }
}

void serial_write_message(const char *message, int len_message) {

	int iter = 0;
    while(iter < len_message) {
    	UARTCharPut(UART1_BASE,*(message + iter));
    	iter++;
    }
}



















