
/*
 * comm_uart.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "comm_uart.h"
#include "lpc17xx_uart.h"
#include "string.h"

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/

#define COMM_UART_PORT LPC_UART2

#define MSG_INIT                "1 - INICIALIZAR MOTOR\r\n"
#define MSG_CONFIG_SPEED        "2 - CONFIGURAR VELOCIDAD\r\n"
#define MSG_CONFIG_LIGHT        "3 - CONFUGRAR SENSOR DE LUZ\r\n"
#define MSG_CONFIG_MOVE         "4 - CONFIGURAR SENSOR DE MOVIMIENTO\r\n"
#define MSG_CLOSE               "5 - CERRAR PUERTA\r\n"
#define MSG_OPEN                "6 - ABRIR PUERTA\r\n"
#define MSG_STOP                "7 - DETENER PUERTA\r\n"

#define MSG_CONFIG_SPEED_LOW    "l - VELOCIDAD BAJA\r\n"
#define MSG_CONFIG_SPEED_MED    "m - VELOCIDAD MEDIA\r\n"
#define MSG_CONFIG_SPEED_HIGH   "h - VELOCIDAD ALTA\r\n"
#define MSG_CONFIG_ENABLE       "e - HABILITAR\r\n"
#define MSG_CONFIG_DISABLE      "d - DEHABILITAR\r\n"

#define MSG_INVALID_CMD         "COMANDO INVALIDO\r\n"

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/

enum CMD
{
    CMD_INIT = '1',
    CMD_CONFIG_SPEED = '2',
    CMD_CONFIG_LIGHT = '3',
    CMD_CONFIG_MOVE = '4',
    CMD_CLOSE = '5',
    CMD_OPEN = '6',
    CMD_STOP = '7',
};
enum ARGS
{
    CMD_LOW_SPEED = 'l',
    CMD_MED_SPEED = 'm',
    CMD_HIGH_SPEED = 'h',
    CMD_ENABLE = 'e',
    CMD_DISABLE = 'd',
};

enum evt
{
    INIT_SEQUENCE,
    CONFIG_SPEED,
    CONFIG_SENSOR_LIGHT,
    CONFIG_SENSOR_MOVE,
    CLOSE,
    OPEN,
    STOP,
};

enum
{
    SHOWING_MENU,
    WAITING_CMD,
    WAITING_SPEED,
    WAITING_LIGHT_CONFIG,
    WAITING_MOVE_CONFIG,
};

/********************************************************************
 *                      STRUCTS
 ********************************************************************/

/********************************************************************
 *                      VARIABLES LOCALES
 ********************************************************************/

static uint8_t byte_rcv;
static uint8_t new_msg;
static uint8_t next_state;
static uint8_t current_state;
static uint8_t send_menu;
/********************************************************************
 *                      PROTOTIPO FUNCIONES LOCALES
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/
void move_fsm(void)
{
    if (!new_msg)
        return;
    
    if (byte_rcv =='\r' || byte_rcv =='\n' )
        return;    

    switch (current_state)
    {
    case SHOWING_MENU:
        send_menu = TRUE;
        next_state = WAITING_CMD;
        break;
    case WAITING_CMD:
        switch (byte_rcv)
        {
        case CMD_INIT:
            // MOTIR_CTRL_init();
            break;
        case CMD_CONFIG_SPEED:
            next_state = WAITING_SPEED;
            break;
        case CMD_CONFIG_LIGHT:
            next_state = WAITING_LIGHT_CONFIG;
            break;
        case CMD_CONFIG_MOVE:
            next_state = WAITING_MOVE_CONFIG;
            break;
        case CMD_CLOSE:
            // MOTOR_CTRL_close();
            next_state = WAITING_CMD;
            break;
        case CMD_OPEN:
            // MOTOR_CTRL_open();
            next_state = WAITING_CMD;
            break;
        case CMD_STOP:
            // MOTOR_CTRL_stop();
            next_state = WAITING_CMD;
            break;

        default:
            next_state = SHOWING_MENU;
            UART_Send(COMM_UART_PORT, (uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD, BLOCKING);
            break;
        }
    case WAITING_SPEED:
        switch (byte_rcv)
        {
        case CMD_LOW_SPEED:
            // MOTOR_CTRL_set_speed(LOW)
            next_state = WAITING_CMD;
            break;
        case CMD_MED_SPEED:
            // MOTOR_CTRL_set_speed(LOW)
            next_state = WAITING_CMD;
            break;
        case CMD_HIGH_SPEED:
            // MOTOR_CTRL_set_speed(LOW)
            next_state = WAITING_CMD;
            break;

        default:
            next_state = SHOWING_MENU;
            UART_Send(COMM_UART_PORT, (uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD, BLOCKING);
            break;
        }

        break;
    case WAITING_LIGHT_CONFIG:
        switch (byte_rcv)
        {
        case CMD_ENABLE:
            // LIGHT_CTRL_set_speed(LOW)
            break;
        case CMD_DISABLE:
            // LIGHT_CTRL_set_speed(LOW)
            break;

        default:
            next_state = SHOWING_MENU;
            UART_Send(COMM_UART_PORT, (uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD, BLOCKING);
            break;
        }
        break;
    case WAITING_MOVE_CONFIG:
        switch (byte_rcv)
        {
        case CMD_ENABLE:
            // MOVE_CTRL_set_speed(LOW)
            break;
        case CMD_DISABLE:
            // MOVE_CTRL_set_speed(LOW)
            break;

        default:
            next_state = SHOWING_MENU;
            UART_Send(COMM_UART_PORT, (uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD, BLOCKING);
            break;
        }
        break;
    default:
        break;
    }
    next_state = current_state;
}



void show_menu(void)
{
    if (!send_menu)
        return;
    send_menu = FALSE;

    switch (current_state)
    {
    case WAITING_CMD:
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_INIT, sizeof MSG_INIT, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_SPEED, sizeof MSG_CONFIG_SPEED, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_LIGHT, sizeof MSG_CONFIG_LIGHT, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_MOVE, sizeof MSG_CONFIG_MOVE, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CLOSE, sizeof MSG_CLOSE, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_OPEN, sizeof MSG_OPEN, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_STOP, sizeof MSG_STOP, BLOCKING);
        break;
    case WAITING_SPEED:
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_SPEED_LOW, sizeof MSG_CONFIG_SPEED_LOW, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_SPEED_MED, sizeof MSG_CONFIG_SPEED_MED, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_SPEED_HIGH, sizeof MSG_CONFIG_SPEED_HIGH, BLOCKING);
        break;
    case WAITING_LIGHT_CONFIG:
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_DISABLE, sizeof MSG_CONFIG_DISABLE, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_ENABLE, sizeof MSG_CONFIG_ENABLE, BLOCKING);
        break;
    case WAITING_MOVE_CONFIG:
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_DISABLE, sizeof MSG_CONFIG_DISABLE, BLOCKING);
        UART_Send(COMM_UART_PORT, (uint8_t *)MSG_CONFIG_ENABLE, sizeof MSG_CONFIG_ENABLE, BLOCKING);
        break;
    default:
        break;
    }
        
}

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void COMM_UART_init(void)
{
    UART_CFG_Type UARTConfigStruct;
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;

    // configuración por defecto:
    UART_ConfigStructInit(&UARTConfigStruct);
    // inicializa periférico
    UART_Init(COMM_UART_PORT, &UARTConfigStruct);
    // Inicializa FIFO
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
    UART_FIFOConfig(COMM_UART_PORT, &UARTFIFOConfigStruct);
    // Habilita interrupción por el RX del UART
    UART_IntConfig(COMM_UART_PORT, UART_INTCFG_RBR, ENABLE);
    // Habilita interrupción por el estado de la linea UART
    UART_IntConfig(COMM_UART_PORT, UART_INTCFG_RLS, ENABLE);
    // Habilita interrupción por UART2
    NVIC_EnableIRQ(UART2_IRQn);
}

void COMM_UART_loop(void)
{
    move_fsm();
    show_menu();
}

void UART2_IRQHandler(void)
{
    uint32_t intsrc, tmp, tmp1;
    // Determina la fuente de interrupción
    intsrc = UART_GetIntId(COMM_UART_PORT);
    tmp = intsrc & UART_IIR_INTID_MASK;
    // Eval�a Line Status
    if (tmp == UART_IIR_INTID_RLS)
    {
        tmp1 = UART_GetLineStatus(COMM_UART_PORT);
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
        // ingresa a un Loop infinito si hay error
        if (tmp1)
        {
            while (1)
            {
            };
        }
    }
    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
    {
        UART_Receive(COMM_UART_PORT, &byte_rcv, sizeof(byte_rcv), NONE_BLOCKING);
        new_msg = TRUE;
    }
    return;
}
