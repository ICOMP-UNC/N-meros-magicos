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

typedef enum
{
    MOTOR_CTRL_LOW_SPEED,
    MOTOR_CTRL_MED_SPEED,
    MOTOR_CTRL_HIGH_SPEED,
} MOTOR_CTRL_SPEED_t;

typedef enum
{
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

void MOTOR_CTRL_init(void);
void MOTOR_CTRL_loop(void);

void MOTOR_CTRL_start_routine(void);
void MOTOR_CTRL_stop(void);
void MOTOR_CTRL_open(void);
void MOTOR_CTRL_close(void);
void MOTOR_CTRL_set_speed(MOTOR_CTRL_SPEED_t speed);
void MOTOR_CTRL_switch_1(void);
void MOTOR_CTRL_switch_2(void);

uint32_t MOTOR_CTRL_get_status(void);
int32_t MOTOR_CTRL_get_abs_pos(void);
MOTOR_states_t MOTOR_CTRL_get_fsm_state(void);
void MOTOR_CTRL_timers(void);
MOTOR_states_t MOTOR_CTRL_get_step(void);

#endif
