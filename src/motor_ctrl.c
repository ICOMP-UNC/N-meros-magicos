
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

#define STEP_TIMEOUT_HIGH 2
#define STEP_TIMEOUT_MED  4
#define STEP_TIMEOUT_LOW  8


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

static void motor_on(void)
{
    int steps_matrix [4][4] =
    //  T1, T2, T3, T4
    {{  0,  1,  1,  1},     //SIGNAL_1
     {  1,  0,  1,  1},     //SIGNAL_2
     {  1,  1,  0,  1},     //SIGNAL_3
     {  1,  1,  1,  0}};    //SIGNAL_4

    if (steps_matrix[SIGNAL_1][step])
        PORT_motor_signal_1_on();
    else
        PORT_motor_signal_1_off();
        
    if (steps_matrix[SIGNAL_2][step])
        PORT_motor_signal_2_on();
    else
        PORT_motor_signal_2_off();
    
    if (steps_matrix[SIGNAL_3][step])
        PORT_motor_signal_3_on();
    else
        PORT_motor_signal_3_off();
        
    if (steps_matrix[SIGNAL_4][step])
        PORT_motor_signal_4_on();
    else
        PORT_motor_signal_4_off();
        
}

static void motor_off(void)
{
    PORT_motor_signal_1_off();
    PORT_motor_signal_2_off();
    PORT_motor_signal_3_off();
    PORT_motor_signal_4_off();
    evt = EV_FINISH_MOVE;
}

static void motor_move(MOTOR_dir_t dir)
{
    motor_on();
    if (dir == DIR_LEFT)
    {
        if (step_timer == 0)
        {
            step_timer = step_timeout;
            step--;
            current_absoulute_pos--;
            if (step == 0xFF)
                step = 3;
        }
    }
    else if (dir == DIR_RIGHT)
    {
        if (step_timer == 0)
        {
            step_timer = step_timeout;
            step++;
            current_absoulute_pos++;
            if (step == 4)
                step = 0;
        }    
    }
}

void motor_move_to_pos(int32_t relative_pos)
{
    int32_t error, pos;
    pos = relative_pos + offset_pos;

    error = pos - current_absoulute_pos;
    if ((error > 0 && error < POS_ERROR) ||
        (error < 0 && (-error) < POS_ERROR)
    )
    {
        evt = EV_FINISH_MOVE;
        return;
    }

    if (pos > current_absoulute_pos)
    {
        evt = EV_NONE;
        motor_move(DIR_RIGHT);
    }

    if (pos < current_absoulute_pos)
    {
        evt = EV_NONE;
        motor_move(DIR_LEFT);
    }
}

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
            motor_move_to_pos(initial_pos);
        }
        else if (status & MOTOR_STATUS_LIMIT_1)
        {
            offset_pos = limit_1_pos;
            initial_pos = 0;
            motor_move(DIR_RIGHT);
        }
        else
        {
            motor_move(DIR_LEFT);
        }
        
        break;
    case MOTOR_MOVING:
        motor_move_to_pos(target_pos);
        break;
    case MOTOR_STOPPING:
        motor_off();
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
    PORT_init_signals();
    PORT_init_switches();

    current_state = MOTOR_IDLE;
    evt = EV_START_ROUTINE;
    step_timeout = STEP_TIMEOUT_HIGH;
    current_absoulute_pos = 0;
    step = 0;
}

void MOTOR_CTRL_loop(void)
{
    move_fsm();
    execute();
}

void MOTOR_CTRL_start_routine(void)
{
    evt = EV_START_ROUTINE;
}

void MOTOR_CTRL_stop(void)
{
    evt = EV_STOP;
}

void MOTOR_CTRL_open(void)
{
    evt = EV_OPEN;
}

void MOTOR_CTRL_close(void)
{
    evt = EV_CLOSE;
}

void MOTOR_CTRL_set_speed(MOTOR_CTRL_SPEED_t speed)
{
    if (speed == MOTOR_CTRL_HIGH_SPEED)
        step_timeout = STEP_TIMEOUT_HIGH;
    else if (speed == MOTOR_CTRL_MED_SPEED)
        step_timeout = STEP_TIMEOUT_MED;
    else if (speed == MOTOR_CTRL_LOW_SPEED)
        step_timeout = STEP_TIMEOUT_LOW;
}



void MOTOR_CTRL_timers(void)
{
    if(step_timer)
    {
        step_timer--;
    }
}

void MOTOR_CTRL_switch_1(void)
{
    if (!(status & MOTOR_STATUS_LIMIT_1))
    {
        status |= MOTOR_STATUS_LIMIT_1;
        limit_1_pos = current_absoulute_pos;
    }
}

void MOTOR_CTRL_switch_2(void)
{
    if (!(status & MOTOR_STATUS_LIMIT_2))
    {
        status |= MOTOR_STATUS_LIMIT_2;
        limit_2_pos = current_absoulute_pos;
    }
}

uint32_t MOTOR_CTRL_get_status(void)
{
    return status;
}

int32_t MOTOR_CTRL_get_abs_pos(void)
{
    return current_absoulute_pos;
}

MOTOR_states_t MOTOR_CTRL_get_fsm_state(void)
{
    return current_state;
}

MOTOR_states_t MOTOR_CTRL_get_step(void)
{
    return step;
}
