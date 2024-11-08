
/*
 * motor_ctrl.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "motor_ctrl.h"
#include "port.h"
#include "stdio.h"

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/


#define MOTOR_STATUS_LIMIT_1 0x0001
#define MOTOR_STATUS_LIMIT_2 0x0002

#define STEP_TIMEOUT_HIGH 10
#define STEP_TIMEOUT_MED  20
#define STEP_TIMEOUT_LOW  40


/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/



typedef enum
{
    EV_START_ROUTINE,
    EV_OPEN,
    EV_CLOSE,
    EV_STOP,
    EV_FINISH_MOVE,
    EV_NONE,
} MOTOR_events_t;

typedef enum
{
    DIR_LEFT,
    DIR_RIGHT,
} MOTOR_dir_t;

enum
{
    SIGNAL_1,
    SIGNAL_2,
    SIGNAL_3,
    SIGNAL_4,
};
/********************************************************************
 *                      STRUCTS
 ********************************************************************/


/********************************************************************
 *                      VARIABLES LOCALES
 ********************************************************************/

static MOTOR_states_t current_state;
static MOTOR_events_t evt;
static uint16_t status;
static int32_t initial_pos;
static int32_t target_pos;
static int32_t offset_pos;
static int32_t final_pos;
static int32_t current_absoulute_pos;
static int32_t limit_1_pos;
static int32_t limit_2_pos;
static uint8_t step;
static uint32_t step_timer;
static uint32_t step_timeout;

/********************************************************************
 *                      PROTOTIPO FUNCIONES LOCALES
 ********************************************************************/


/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

static void move_fsm(void)
{
    MOTOR_states_t next_state;
    next_state = current_state;

    switch (current_state)
    {
    case MOTOR_IDLE:
        switch (evt)
        {
        case EV_START_ROUTINE:
            step_timer = step_timeout;
            next_state = MOTOR_START_ROUTINE;
            break;
        case EV_OPEN:
            target_pos = initial_pos;
            next_state = MOTOR_MOVING;
            break;
        case EV_CLOSE:
            target_pos = final_pos;
            next_state = MOTOR_MOVING;
            break;
        case EV_STOP:
            next_state = MOTOR_STOPPING;
            break;
        case EV_NONE:
        case EV_FINISH_MOVE:
            break;
        
        default:
            break;
        }
        break;
    case MOTOR_START_ROUTINE:
        switch (evt)
        {
        case EV_FINISH_MOVE:
            next_state = MOTOR_IDLE;
        case EV_STOP:
            next_state = MOTOR_STOPPING;
            break;
        default:
            break;
        }
        break;
    case MOTOR_MOVING:
        switch (evt)
        {
        case EV_FINISH_MOVE:
            next_state = MOTOR_IDLE;
        case EV_STOP:
            next_state = MOTOR_STOPPING;
            break;
        default:
            break;
        }
        break;
    case MOTOR_STOPPING:
        switch (evt)
        {
        case EV_FINISH_MOVE:
            next_state = MOTOR_IDLE;
            break;
        default:
            break;
        }
        break;
    
    default:
        break;
    }
    current_state = next_state;
    evt = EV_NONE;
}

void execute(void)
{
    switch (current_state)
    {
    case MOTOR_START_ROUTINE:
        // offset = abs - rel -> abs = rel + offset
        if (status & MOTOR_STATUS_LIMIT_2)
        {
            final_pos = limit_2_pos - offset_pos;
        }
        else if (status & MOTOR_STATUS_LIMIT_1)
        {
            offset_pos = limit_1_pos;
            initial_pos = 0;
        }
        else
        {
        }
        
        break;
    case MOTOR_MOVING:
        break;
    case MOTOR_STOPPING:
        break;
    
    default:
        break;
    }
}

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void MOTOR_CTRL_init(void)
{

    current_state = MOTOR_IDLE;
    evt = EV_START_ROUTINE;
    step_timeout = STEP_TIMEOUT_LOW;
    current_absoulute_pos = 0;
    step = 0;
}

void MOTOR_CTRL_loop(void)
{
    move_fsm();
    execute();
}
