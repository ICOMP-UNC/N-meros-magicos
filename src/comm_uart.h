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

void COMM_UART_init(void);
void COMM_UART_loop(void);
void COMM_UART_temp_alarm(void);
void COMM_UART_temp_ok(void);

#endif
