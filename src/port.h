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

#define MOTOR_PORT_SIGNAL  GPIOA
#define MOTOR_PORT_SWITCH  GPIOA
#define MOTOR_PORT_OUTPUT  GPIOA
#define PORT_BUILT_IN_LED  GPIOC

#define MOTOR_SIGNAL_1_MASK GPIO1
#define MOTOR_SIGNAL_2_MASK GPIO2
#define MOTOR_SIGNAL_3_MASK GPIO3
#define MOTOR_SIGNAL_4_MASK GPIO4
#define BUILT_IN_LED_MASK GPIO13

#define MOTOR_SWITCH_1_MASK GPIO5
#define MOTOR_SWITCH_2_MASK GPIO6

#define MOTOR_LED_MASK GPIO7


#define MOTOR_SWITCHES_MASK (MOTOR_SWITCH_1_MASK | MOTOR_SWITCH_2_MASK)
#define MOTOR_SIGNALS_MASK (MOTOR_SIGNAL_1_MASK | MOTOR_SIGNAL_2_MASK | MOTOR_SIGNAL_3_MASK | MOTOR_SIGNAL_4_MASK)
#define MOTOR_OUTPUTS_MASK (MOTOR_LED_MASK)

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
void PORT_built_in_led_off(void);
void PORT_built_in_led_on(void);
void PORT_built_in_led_toggle(void);

void PORT_init_buzzer(void);
void PORT_buzzer_on(void);
void PORT_buzzer_off(void);

#endif
