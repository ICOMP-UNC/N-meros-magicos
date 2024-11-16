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

#define MOTOR_PORT_SIGNAL GPIOA
#define MOTOR_PORT_SWITCH GPIOA
#define MOTOR_PORT_OUTPUT GPIOA
#define PORT_BUILT_IN_LED GPIOC
#define PORT_COOLER       GPIOB

#define MOTOR_SIGNAL_1_MASK GPIO1
#define MOTOR_SIGNAL_2_MASK GPIO2
#define MOTOR_SIGNAL_3_MASK GPIO3
#define MOTOR_SIGNAL_4_MASK GPIO4
#define BUILT_IN_LED_MASK   GPIO13

#define MOTOR_SWITCH_1_MASK GPIO5
#define MOTOR_SWITCH_2_MASK GPIO6

#define MOTOR_LED_MASK GPIO7
#define COOLER_MASK    GPIO0

#define MOTOR_SWITCHES_MASK (MOTOR_SWITCH_1_MASK | MOTOR_SWITCH_2_MASK)
#define MOTOR_SIGNALS_MASK  (MOTOR_SIGNAL_1_MASK | MOTOR_SIGNAL_2_MASK | MOTOR_SIGNAL_3_MASK | MOTOR_SIGNAL_4_MASK)
#define MOTOR_OUTPUTS_MASK  (MOTOR_LED_MASK)

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/

/********************************************************************
 *                      STRUCTS
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

/**
 * @brief Inicializa switches de fin de carrera
 *
 */
void PORT_init_switches(void);

/**
 * @brief Inicializa señales de control del motor
 *
 */
void PORT_init_signals(void);

/**
 * @brief Enciende señal 1 del motor
 *
 */
void PORT_motor_signal_1_on(void);

/**
 * @brief Enciende señal 2 del motor
 */
void PORT_motor_signal_2_on(void);

/**
 * @brief Enciende señal 3 del motor
 */
void PORT_motor_signal_3_on(void);

/**
 * @brief Enciende señal 4 del motor
 */
void PORT_motor_signal_4_on(void);

/**
 * @brief Apaga señal 1 del motor
 */
void PORT_motor_signal_1_off(void);

/**
 * @brief Apaga señal 2 del motor
 */
void PORT_motor_signal_2_off(void);

/**
 * @brief Apaga señal 3 del motor
 */
void PORT_motor_signal_3_off(void);

/**
 * @brief Apaga señal 4 del motor
 */
void PORT_motor_signal_4_off(void);

/**
 * @brief Inicializa salidas LED y cooler
 *
 */
void PORT_init_outputs(void);

/**
 * @brief Apaga el indicador led de motor
 *
 */
void PORT_led_off(void);

/**
 * @brief Enciende el indicador led de motor
 *
 */
void PORT_led_on(void);

/**
 * @brief Apaga el indicador led built-in
 *
 */
void PORT_built_in_led_off(void);

/**
 * @brief Enciende el indicador led de built-in
 *
 */
void PORT_built_in_led_on(void);

/**
 * @brief Togglea el indicador led de built-in
 *
 */
void PORT_built_in_led_toggle(void);

/**
 * @brief Inicializa el buzzer
 *
 */
void PORT_init_buzzer(void);

/**
 * @brief Habilita el buzzer
 *
 */
void PORT_buzzer_on(void);

/**
 * @brief Deshabilita el buzzer
 *
 */
void PORT_buzzer_off(void);

#endif
