
#include <coresrv/syscalls.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <rtl/countof.h>

#include <uart/uart.h>

int uartmain(const char& message)
{
    
    Retcode rc;
    UartHandle uartH = KDF_INVALID_HANDLE;

    printf(strForStdout);

    rc = UartInit();

    if (rc != UART_OK) {
        fprintf(stderr,"UartInit() failed\n");
        return EXIT_FAILURE;
    }

    rc = UartOpenPort("uart0", &uartH);

    if (rc != UART_OK) {
        fprintf(stderr,"UartOpenPort() failed\n");
        return EXIT_FAILURE;
    }

    sleep(1);

    rtl_size_t i;

    for (i = 0; i < rtl_countof(strForUart) - 1; ++i)
        UartWriteByte(uartH, (uint8_t)strForUart[i]);

    return EXIT_SUCCESS;
}
