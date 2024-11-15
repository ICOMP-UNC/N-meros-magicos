/*
 * motor_ctrl.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */
#ifndef __MOTOR_CTRL_H
#define __MOTOR_CTRL_H

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "stdint.h"

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/
#define POS_ERROR 100

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/

typedef enum {
  MOTOR_CTRL_LOW_SPEED,
  MOTOR_CTRL_MED_SPEED,
  MOTOR_CTRL_HIGH_SPEED,
} MOTOR_CTRL_SPEED_t;

typedef enum {
  MOTOR_IDLE,
  MOTOR_START_ROUTINE,
  MOTOR_MOVING,
  MOTOR_STOPPING,
} MOTOR_states_t;

/********************************************************************
 *                      STRUCTS
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

/**
 * @brief Inicializa el módulo de control de motor
 * 
 */
void MOTOR_CTRL_init(void);

/**
 * @brief Bucle principal con maquina de estados
 * 
 */
void MOTOR_CTRL_loop(void);

/**
 * @brief Inicia la rutina de inicio del motor
 * 
 */
void MOTOR_CTRL_start_routine(void);

/**
 * @brief Detiene el movimiento del motor
 * 
 */
void MOTOR_CTRL_stop(void);

/**
 * @brief Abre la compuerta
 * 
 */
void MOTOR_CTRL_open(void);

/**
 * @brief Cierra la compuerta
 * 
 */
void MOTOR_CTRL_close(void);

/**
 * @brief Setea la velocidad de movimiento del motor
 * 
 * @param speed 
 */
void MOTOR_CTRL_set_speed(MOTOR_CTRL_SPEED_t speed);

/**
 * @brief Evento de switch de fin de carrera 1
 * 
 */
void MOTOR_CTRL_switch_1(void);

/**
 * @brief Evento de switch de fin de carrera 2
 * 
 */
void MOTOR_CTRL_switch_2(void);

/**
 * @brief Lee el estado del modulo
 * 
 * @return uint32_t 
 */
uint32_t MOTOR_CTRL_get_status(void);

/**
 * @brief Lee la posición absoluta del motor
 * 
 * @return int32_t 
 */
int32_t MOTOR_CTRL_get_abs_pos(void);

/**
 * @brief Lee el estado actual de la maquina de estados
 * 
 * @return MOTOR_states_t 
 */
MOTOR_states_t MOTOR_CTRL_get_fsm_state(void);

/**
 * @brief Timer del módulo
 * 
 */
void MOTOR_CTRL_timers(void);

/**
 * @brief Devuelve el paso dentro de la matriz de control
 * 
 * @return MOTOR_states_t 
 */
MOTOR_states_t MOTOR_CTRL_get_step(void);

#endif
