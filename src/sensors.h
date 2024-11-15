/*
 * sensors.h
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */
#ifndef __SENSORS_H
#define __SENSORS_H

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "stdint.h"

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
 * @brief Inicializa el sensor de temperatura
 * 
 */
void SENSORS_init(void);

/**
 * @brief Obtiene el historial de temperaturas en forma de string
 * 
 * @param buffer buffer donde se almacenará el string
 * @return uint16_t Longitud del string
 */
uint16_t SENSORS_read_temp_history(char *buffer);

#endif
