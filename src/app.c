
/*
 * app.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "app.h"
#include "motor_ctrl.h"
#include "comm_uart.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/


/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/


/********************************************************************
 *                      STRUCTS
 ********************************************************************/


/********************************************************************
 *                      VARIABLES LOCALES
 ********************************************************************/


/********************************************************************
 *                      PROTOTIPO FUNCIONES LOCALES
 ********************************************************************/


/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

void systemInit(void)
{
    /* Configure the system clock to run at 72 MHz using an 8 MHz external crystal */
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

void configure_gpio(void)
{
    rcc_periph_clock_enable(RCC_GPIOC);
    /* Set PC13 as a push-pull output at 2 MHz */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}
/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void APP_init(void)
{
    systemInit();
    configure_gpio();
    /* Inicializar modulos */
    MOTOR_CTRL_init();
	COMM_UART_init();

}

void APP_loop(void)
{
    gpio_toggle(GPIOC, GPIO13); /* Toggle the LED on PC13 */
		for (int i = 0; i < 1000000; i++)	/* Wait a bit. */
			__asm__("nop");
    MOTOR_CTRL_loop();
    COMM_UART_loop();
}
