/*
 * output.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */
#ifndef __OUTPUT_H
#define __OUTPUT_H

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

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
 *                      FUNCIONES GLOBALES
 ********************************************************************/

/**
 * @brief Inicializa las salidas LED y COOLER Y buzzer
 *
 */
void OUTPUT_init(void);

/**
 * @brief Bucle principal de las salidas
 *
 */
void OUTPUT_loop(void);

/**
 * @brief Enciende el indicador de movimiento
 *
 */
void OUTPUT_led_on(void);

/**
 * @brief Apaga el indicador de movimiento
 *
 */
void OUTPUT_led_off(void);

/**
 * @brief Enciende el cooler
 *
 */
void OUTPUT_cooler_on(void);

/**
 * @brief Apaga el cooler
 *
 */
void OUTPUT_cooler_off(void);

/**
 * @brief Habilita el buzzer
 *
 */
void OUTPUT_buzzer_on(void);

/**
 * @brief Deshabilita el buzzer
 *
 */
void OUTPUT_buzzer_off(void);

/**
 * @brief Timers del módulo
 *
 */
void OUTPUT_timers(void);
#endif
