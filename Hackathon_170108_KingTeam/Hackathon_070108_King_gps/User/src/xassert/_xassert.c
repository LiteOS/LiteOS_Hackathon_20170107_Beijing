#ifndef NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include "usart3.h"
//#include "user_config.h"

//static int uart_putchar(char c, FILE *stream)
//{
//    xuart_unbuf_xmit((uint8_t)c);
//    return 0;
//}

//static FILE xstdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int _xassert(const char* e, const char* file_name, int line_no)
{
        //#ifndef NDEBUG
        //usart3_init(9600);
        //usart3_printf("assertion warning: %s, file %s, line %d\r\n", e, file_name, line_no);
        //#endif
        return -1;
}
#endif  /* #ifndef NDEBUG */

