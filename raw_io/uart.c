#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "string.h"
#include "Xcom_master.h"
#include "uart.h"

/*************************************************************************
 Function: uart_init()
 Purpose:  initialize UART and set baudrate
 Input:    baudrate using macro UART_BAUD_SELECT()
 Returns:  none
 **************************************************************************/
void
uart_init (unsigned int baudrate)
{

}/* uart_init */

/*************************************************************************
 Function: uart_getc()
 Purpose:  return byte from ringbuffer
 Returns:  lower byte:  received byte from ringbuffer
 higher byte: last receive error
 **************************************************************************/
unsigned int
uart_getc (void)
{
  return (0);

}/* uart_getc */

/*************************************************************************
 Function: uart_putc()
 Purpose:  write byte to ringbuffer for transmitting via UART
 Input:    byte to be transmitted
 Returns:  none
 **************************************************************************/
void
uart_putc (unsigned char data)
{
  Xcom_cmd (XCMD_SERIAL_SEND, 1, &data);
}/* uart_putc */

/*************************************************************************
 Function: uart_puts()
 Purpose:  transmit string to UART
 Input:    string to be transmitted
 Returns:  none
 **************************************************************************/
void
uart_puts (const char *s)
{
  Xcom_cmd (XCMD_SERIAL_SEND, strlen (s)+1, s);
}/* uart_puts */

/*************************************************************************
 Function: uart_puts_p()
 Purpose:  transmit string from program memory to UART
 Input:    program memory string to be transmitted
 Returns:  none
 **************************************************************************/
void
uart_puts_p (const char *progmem_s)
{
  register uint8_t i = 0;
  char p[XCOM_BUFSIZE];

  do
    {
      p[i] = pgm_read_byte(progmem_s++);
    }
  while(p[i++] != 0);
  uart_puts (p);

}/* uart_puts_p */

/*************************************************************************
 Function: uart_available()
 Purpose:  Determine the number of bytes waiting in the receive buffer
 Input:    None
 Returns:  Integer number of bytes in the receive buffer
 **************************************************************************/
int
uart_available (void)
{
  return 0;
}/* uart_available */

/*************************************************************************
 Function: uart_flush()
 Purpose:  Flush bytes waiting the receive buffer.  Acutally ignores them.
 Input:    None
 Returns:  None
 **************************************************************************/
void
uart_flush (void)
{
}/* uart_flush */

/*
 * these functions are only for ATmegas with two USART
 */
