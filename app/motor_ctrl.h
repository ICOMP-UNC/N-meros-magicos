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

#endif
