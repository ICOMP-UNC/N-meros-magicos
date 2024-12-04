
/*
 * output.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "output.h"
#include "port.h"
#include "stdint.h"

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/
#define LED_TIMEOUT    500
#define BUZZER_TIMEOUT 1000

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/

/********************************************************************
 *                      STRUCTS
 ********************************************************************/

/********************************************************************
 *                      VARIABLES LOCALES
 ********************************************************************/

static uint32_t built_in_led_timer;
static uint32_t buzzer_timer;
static uint8_t buzzer_state;
static uint8_t buzzer_enabled;

/********************************************************************
 *                      PROTOTIPO FUNCIONES LOCALES
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void OUTPUT_init(void)
{
    PORT_init_outputs();
    PORT_init_buzzer();
    PORT_buzzer_off();
    PORT_init_cooler();
    built_in_led_timer = 1000;
}

void OUTPUT_led_on(void)
{
    PORT_led_on();
}

void OUTPUT_led_off(void)
{
    PORT_led_off();
}

void OUTPUT_cooler_on(void)
{
    PORT_cooler_on();
}

void OUTPUT_cooler_off(void)
{
    PORT_cooler_off();
}

void OUTPUT_buzzer_on(void)
{
    buzzer_enabled = 1;
}

void OUTPUT_buzzer_off(void)
{
    buzzer_enabled = 0;
    PORT_buzzer_off();
}

void OUTPUT_set_buzzer_level(uint16_t level)
{
    PORT_set_buzzer_level(level);
}

void OUTPUT_loop(void)
{
    if (built_in_led_timer == 0)
    {
        built_in_led_timer = LED_TIMEOUT;
        PORT_built_in_led_toggle();
    }
    if (buzzer_enabled && buzzer_timer == 0)
    {
        if (buzzer_state)
        {
            buzzer_timer = BUZZER_TIMEOUT;
            buzzer_state = 0;
            PORT_buzzer_off();
        }
        else
        {
            buzzer_timer = BUZZER_TIMEOUT;
            buzzer_state = 1;
            PORT_buzzer_on();
        }
    }
}

void OUTPUT_timers(void)
{
    if (built_in_led_timer)
    {
        built_in_led_timer--;
    }

    if (buzzer_timer)
    {
        buzzer_timer--;
    }
}