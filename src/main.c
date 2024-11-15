/*
===============================================================================
 Name        : ed3_integrador.c
 Author      : Horacio Rueda
 Version     :
 Copyright   : $(copyright)
 Description : Aplicación que controla un motor paso a paso por medio de
               Interfaz UART con monitoreo de sensor de temperatura
===============================================================================
*/

#include "app.h"

int main(void) {
  APP_init();

  while (1) {
    APP_loop();
  }
  return 0;
}
