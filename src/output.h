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

void OUTPUT_init(void);
void OUTPUT_loop(void);
void OUTPUT_led_on(void);
void OUTPUT_led_off(void);
void OUTPUT_cooler_on(void);
void OUTPUT_cooler_off(void);
void OUTPUT_buzzer_on(void);
void OUTPUT_buzzer_off(void);
void OUTPUT_timers(void);
#endif
