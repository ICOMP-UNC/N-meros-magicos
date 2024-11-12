/*
 * port.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */
#ifndef __PORT_H
#define __PORT_H

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/


/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/


#define FALLING_EDGE 1

#define MOTOR_PORT_SIGNAL  0
#define MOTOR_PORT_SWITCH  0
#define MOTOR_PORT_OUTPUT  0

#define MOTOR_SIGNAL_1 10
#define MOTOR_SIGNAL_2 11
#define MOTOR_SIGNAL_3 12
#define MOTOR_SIGNAL_4 13
#define MOTOR_SWITCH_1 14
#define MOTOR_SWITCH_2 15

#define MOTOR_LED 16
#define MOTOR_BUZZER 17

#define MOTOR_SIGNAL_1_MASK (1 << MOTOR_SIGNAL_1)
#define MOTOR_SIGNAL_2_MASK (1 << MOTOR_SIGNAL_2)
#define MOTOR_SIGNAL_3_MASK (1 << MOTOR_SIGNAL_3)
#define MOTOR_SIGNAL_4_MASK (1 << MOTOR_SIGNAL_4)

#define MOTOR_SWITCH_1_MASK (1 << MOTOR_SWITCH_1)
#define MOTOR_SWITCH_2_MASK (1 << MOTOR_SWITCH_2)

#define MOTOR_LED_MASK (1 << MOTOR_LED)
#define MOTOR_BUZZER_MASK (1 << MOTOR_BUZZER)


#define MOTOR_SWITCHES_MASK (MOTOR_SWITCH_1_MASK | MOTOR_SWITCH_2_MASK)
#define MOTOR_SIGNALS_MASK (MOTOR_SIGNAL_1_MASK | MOTOR_SIGNAL_2_MASK | MOTOR_SIGNAL_3_MASK | MOTOR_SIGNAL_4_MASK)

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/


/********************************************************************
 *                      STRUCTS
 ********************************************************************/


/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void PORT_init_switches(void);
void PORT_init_signals(void);

void PORT_motor_signal_1_on(void);
void PORT_motor_signal_2_on(void);
void PORT_motor_signal_3_on(void);
void PORT_motor_signal_4_on(void);
void PORT_motor_signal_1_off(void);
void PORT_motor_signal_2_off(void);
void PORT_motor_signal_3_off(void);
void PORT_motor_signal_4_off(void);

void PORT_init_outputs(void);
void PORT_led_off(void);
void PORT_led_on(void);
void PORT_buzzer_on(void);
void PORT_buzzer_off(void);

#endif
