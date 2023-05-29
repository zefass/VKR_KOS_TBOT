#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <string>
#include <iostream>
#include "gpioled.h"

#ifdef __arm__
#include <bsp/bsp.h>
#endif

#define GPIO_PIN_NUM_IN1 17U
#define GPIO_PIN_NUM_IN2 6U
#define UART_TXD 14
#define UART_RXD 15
#define UART_GND 9U

#define HIGH 1
#define LOW 0



void light(GpioHandle* handle) {
    fprintf(stderr, "Turning on the LED...\n");
    GpioOut(*handle, GPIO_PIN_NUM_IN1, HIGH);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, HIGH);
}

void dark(GpioHandle* handle) {
    fprintf(stderr, "Turning of the LED...\n");
    GpioOut(*handle, GPIO_PIN_NUM_IN1, LOW);
    GpioOut(*handle, GPIO_PIN_NUM_IN2, LOW);
}

int gpiomain(const std::string& cmd)
{
    fprintf(stderr, "Start GPIO_output test\n");

#ifdef __arm__

    {
        BspError rc = BspInit(NULL);
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to initialize BSP\n");
            return EXIT_FAILURE;
        }

        rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to set mux configuration for gpio0 module\n");
            return EXIT_FAILURE;
        }
    }
    
#endif

    if (GpioInit())
    {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    /* initialize and setup gpio0 port */
    GpioHandle handle = NULL;
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE)
    {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }

    GpioSetMode(handle, GPIO_PIN_NUM_IN1, GPIO_DIR_OUT);
    GpioSetMode(handle, GPIO_PIN_NUM_IN2, GPIO_DIR_OUT);


    fprintf(stderr, "Starting move (%s)\n", cmd);
    GpioHandle* p_handle = &handle;
    
   

    if(cmd == light)
    {
      light(p_handle);
    }
    
    if(cmd == dark)
    {
      dark(p_handle);
    }
    

    fprintf(stderr, "Test finished.\n");

    return EXIT_SUCCESS;
}
