/*
 * comm_uart.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */
#ifndef __COMM_UART_H
#define __COMM_UART_H

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
 * @brief Inicializa el módulo de comunicación UART
 *
 */
void COMM_UART_init(void);

/**
 * @brief Bucle principal con máquina de estados
 *
 */
void COMM_UART_loop(void);

/**
 * @brief Envía por UART el mensaje de alarma de temperatura elevada
 *
 */
void COMM_UART_temp_alarm(void);

/**
 * @brief Envía por UART el mensaje de temperatura OK
 *
 */
void COMM_UART_temp_ok(void);

/**
 * @brief Envía por UART el mensaje de error de motor
 *
 */
void COMM_UART_motor_error(void);

#endif
