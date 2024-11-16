
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
#include "comm_uart.h"
#include "motor_ctrl.h"
#include "output.h"
#include "sensors.h"
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
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
static void systemInit(void);
static void systick_setup(void);
void sys_tick_handler(void);

/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

static void systemInit(void)
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

void sys_tick_handler(void)
{
    MOTOR_CTRL_timers();
    OUTPUT_timers();
}

static void systick_setup(void)
{
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(72000 - 1);
    systick_interrupt_enable();
    systick_counter_enable();
}
/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void APP_init(void)
{
    /* Inicializar sistema */
    systemInit();
    systick_setup();

    /* Inicializar modulos */
    MOTOR_CTRL_init();
    COMM_UART_init();
    OUTPUT_init();
    SENSORS_init();
}

void APP_loop(void)
{
    MOTOR_CTRL_loop();
    COMM_UART_loop();
    OUTPUT_loop();
}
