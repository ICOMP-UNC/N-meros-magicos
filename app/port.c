
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

#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

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


/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void PORT_init_switches(void)
{
    PINSEL_CFG_Type config = {
        .Portnum = MOTOR_PORT_SWITCH,
        .Funcnum = PINSEL_FUNC_0,
        .Pinmode = PINSEL_PINMODE_PULLUP,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
    };

    config.Pinnum = MOTOR_SWITCH_1;
    PINSEL_ConfigPin(&config);

    config.Pinnum = MOTOR_SWITCH_1;
    PINSEL_ConfigPin(&config);

    GPIO_SetDir(MOTOR_PORT_SWITCH, MOTOR_SWITCHES_MASK, GPIO_DIR_INPUT);
    GPIO_IntCmd(MOTOR_PORT_SWITCH, MOTOR_SWITCHES_MASK, FALLING_EDGE);
    GPIO_ClearInt(MOTOR_PORT_SWITCH, MOTOR_SWITCHES_MASK);
    NVIC_EnableIRQ(EINT3_IRQn);
}


void PORT_init_signals(void)
{
    PINSEL_CFG_Type config = {
        .Portnum = MOTOR_PORT_SIGNAL,
        .Funcnum = PINSEL_FUNC_0,
        .Pinmode = PINSEL_PINMODE_NORMAL,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
    };

    config.Pinnum = MOTOR_SIGNAL_1;
    PINSEL_ConfigPin(&config);

    config.Pinnum = MOTOR_SIGNAL_2;
    PINSEL_ConfigPin(&config);

    config.Pinnum = MOTOR_SIGNAL_3;
    PINSEL_ConfigPin(&config);

    config.Pinnum = MOTOR_SIGNAL_4;
    PINSEL_ConfigPin(&config);

    GPIO_SetDir(MOTOR_PORT_SIGNAL, MOTOR_SIGNALS_MASK, GPIO_DIR_OUTPUT);
}

void PORT_motor_signal_1_on(void)
{
    GPIO_SetValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_1_MASK);
}
void PORT_motor_signal_2_on(void)
{
    GPIO_SetValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_2_MASK);
}
void PORT_motor_signal_3_on(void)
{
    GPIO_SetValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_3_MASK);
}
void PORT_motor_signal_4_on(void)
{
    GPIO_SetValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_4_MASK);
}

void PORT_motor_signal_1_off(void)
{
    GPIO_ClearValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_1_MASK);
}
void PORT_motor_signal_2_off(void)
{
    GPIO_ClearValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_2_MASK);
}
void PORT_motor_signal_3_off(void)
{
    GPIO_ClearValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_3_MASK);
}
void PORT_motor_signal_4_off(void)
{
    GPIO_ClearValue(MOTOR_PORT_SIGNAL,MOTOR_SIGNAL_4_MASK);
}
