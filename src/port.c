
/*
 * port.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "port.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/

#define RCC_PORT_SWITCH RCC_GPIOA
#define RCC_PORT_SIGNANLS RCC_GPIOA
#define RCC_PORT_OUTPUTS RCC_GPIOA
#define RCC_PORT_BUILT_IN_LED RCC_GPIOC

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


/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void PORT_init_switches(void)
{
    rcc_periph_clock_enable(RCC_PORT_SWITCH);
    gpio_set_mode(MOTOR_PORT_SWITCH, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, MOTOR_SWITCHES_MASK);
    gpio_set(MOTOR_PORT_SWITCH, MOTOR_SWITCHES_MASK);
}


void PORT_init_signals(void)
{
    rcc_periph_clock_enable(RCC_PORT_SIGNANLS);
    gpio_set_mode(MOTOR_PORT_SIGNAL, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, MOTOR_SIGNALS_MASK);
    gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNALS_MASK);
}

void PORT_init_outputs(void)
{
    rcc_periph_clock_enable(RCC_PORT_OUTPUTS);
    rcc_periph_clock_enable(RCC_PORT_BUILT_IN_LED);

    gpio_set_mode(MOTOR_PORT_OUTPUT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, MOTOR_OUTPUTS_MASK);
    gpio_clear(MOTOR_PORT_OUTPUT, MOTOR_OUTPUTS_MASK);

    gpio_set_mode(PORT_BUILT_IN_LED, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, BUILT_IN_LED_MASK);
}
void PORT_motor_signal_1_on(void)
{
    gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_1_MASK);
}

void PORT_motor_signal_2_on(void)
{
    gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_2_MASK);
}

void PORT_motor_signal_3_on(void)
{
    gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_3_MASK);
}

void PORT_motor_signal_4_on(void)
{
    gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_4_MASK);
}

void PORT_motor_signal_1_off(void)
{
    gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_1_MASK);
}

void PORT_motor_signal_2_off(void)
{
    gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_2_MASK);
}

void PORT_motor_signal_3_off(void)
{
    gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_3_MASK);
}

void PORT_motor_signal_4_off(void)
{
    gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_4_MASK);
}


void PORT_led_on(void)
{
    gpio_set(MOTOR_PORT_OUTPUT, MOTOR_LED_MASK);
}
void PORT_led_off(void)
{
    gpio_clear(MOTOR_PORT_OUTPUT, MOTOR_LED_MASK);
}

void PORT_built_in_led_on(void)
{
    gpio_set(PORT_BUILT_IN_LED, BUILT_IN_LED_MASK);
}
void PORT_built_in_led_off(void)
{
    gpio_clear(PORT_BUILT_IN_LED, BUILT_IN_LED_MASK);
}

void PORT_built_in_led_toggle(void)
{
    gpio_toggle(PORT_BUILT_IN_LED, BUILT_IN_LED_MASK);
}

void PORT_buzzer_on(void)
{
    gpio_set(MOTOR_PORT_OUTPUT, MOTOR_BUZZER_MASK);
}
    
void PORT_buzzer_off(void)
{
    gpio_clear(MOTOR_PORT_OUTPUT, MOTOR_BUZZER_MASK);
}