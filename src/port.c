
/*
 * port.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/

#include "port.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/

#define RCC_PORT_SWITCH RCC_GPIOA
#define RCC_PORT_SIGNANLS RCC_GPIOA
#define RCC_PORT_OUTPUTS RCC_GPIOA
#define RCC_PORT_BUZZER RCC_GPIOB
#define RCC_PORT_COOLER RCC_GPIOB
#define RCC_PORT_BUILT_IN_LED RCC_GPIOC

#define BUZZER_PORT GPIOB
#define BUZZER_MASK GPIO9

#define BUZZER_MAX_PERIOD 40909

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/

/********************************************************************
 *                      STRUCTS
 ********************************************************************/

/********************************************************************
 *                      VARIABLES LOCALES
 ********************************************************************/

/********************************************************************
 *                      PROTOTIPO FUNCIONES LOCALES
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void PORT_init_switches(void) {
  rcc_periph_clock_enable(RCC_PORT_SWITCH);
  gpio_set_mode(MOTOR_PORT_SWITCH, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                MOTOR_SWITCHES_MASK);
  gpio_set(MOTOR_PORT_SWITCH, MOTOR_SWITCHES_MASK);

  rcc_periph_clock_enable(RCC_AFIO);

  nvic_enable_irq(NVIC_EXTI9_5_IRQ);

  /* Configure the EXTI subsystem. */
  exti_select_source(EXTI5 | EXTI6, GPIOA);
  exti_set_trigger(EXTI5 | EXTI6, EXTI_TRIGGER_FALLING);
  exti_enable_request(EXTI5 | EXTI6);
}

void PORT_init_signals(void) {
  rcc_periph_clock_enable(RCC_PORT_SIGNANLS);
  gpio_set_mode(MOTOR_PORT_SIGNAL, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, MOTOR_SIGNALS_MASK);
  gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNALS_MASK);
}

void PORT_init_outputs(void) {
  rcc_periph_clock_enable(RCC_PORT_OUTPUTS);
  rcc_periph_clock_enable(RCC_PORT_BUILT_IN_LED);
  rcc_periph_clock_enable(RCC_PORT_COOLER);

  gpio_set_mode(MOTOR_PORT_OUTPUT, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, MOTOR_LED_MASK);
  gpio_clear(MOTOR_PORT_OUTPUT, MOTOR_LED_MASK);

  gpio_set_mode(PORT_COOLER, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                COOLER_MASK);
  gpio_clear(PORT_COOLER, COOLER_MASK);

  gpio_set_mode(PORT_BUILT_IN_LED, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, BUILT_IN_LED_MASK);
}

void PORT_init_buzzer(void) {
  /* Se configuran los pines de los puertos correspondientes */
  rcc_periph_clock_enable(RCC_PORT_BUZZER);
  rcc_periph_clock_enable(RCC_TIM4);
  gpio_set_mode(BUZZER_PORT,                    // Puerto correspondiente
                GPIO_MODE_OUTPUT_2_MHZ,         // Máxima velocidad de switcheo
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, // Función alternativa
                BUZZER_MASK); // Pines asociados al OC2, OC3 y OC4
  /* Se configura el TIM como PWM alineado al centro */
  timer_set_mode(TIM4,                 // Timer general 4
                 TIM_CR1_CKD_CK_INT,   // Clock interno como fuente
                 TIM_CR1_CMS_CENTER_1, // Modo centrado
                 TIM_CR1_DIR_UP);      // Indistinto, esto es ignorado...
  /*  Seteamos la cuenta del Timer

      Recordemos que como el PWM está alineado al centro el timer
      cuenta para arriba y luego para abajo, por lo tanto, debemos
      dividir la frecuencia x2.
  */
  timer_set_period(TIM4, BUZZER_MAX_PERIOD - 1); // 72M/40909 = 1760hz
  // Configuramos las salidas del timer:
  timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM2);
  // Habilitamos las salidas de los canales:
  timer_enable_oc_output(TIM4, TIM_OC4);

  timer_set_oc_value(TIM4, TIM_OC4, BUZZER_MAX_PERIOD / 2);

  timer_enable_counter(TIM4);
}

void PORT_motor_signal_1_on(void) {
  gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_1_MASK);
}

void PORT_motor_signal_2_on(void) {
  gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_2_MASK);
}

void PORT_motor_signal_3_on(void) {
  gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_3_MASK);
}

void PORT_motor_signal_4_on(void) {
  gpio_set(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_4_MASK);
}

void PORT_motor_signal_1_off(void) {
  gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_1_MASK);
}

void PORT_motor_signal_2_off(void) {
  gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_2_MASK);
}

void PORT_motor_signal_3_off(void) {
  gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_3_MASK);
}

void PORT_motor_signal_4_off(void) {
  gpio_clear(MOTOR_PORT_SIGNAL, MOTOR_SIGNAL_4_MASK);
}

void PORT_led_on(void) { gpio_set(MOTOR_PORT_OUTPUT, MOTOR_LED_MASK); }
void PORT_led_off(void) { gpio_clear(MOTOR_PORT_OUTPUT, MOTOR_LED_MASK); }

void PORT_cooler_on(void) { gpio_set(PORT_COOLER, COOLER_MASK); }
void PORT_cooler_off(void) { gpio_clear(PORT_COOLER, COOLER_MASK); }

void PORT_built_in_led_on(void) {
  gpio_set(PORT_BUILT_IN_LED, BUILT_IN_LED_MASK);
}
void PORT_built_in_led_off(void) {
  gpio_clear(PORT_BUILT_IN_LED, BUILT_IN_LED_MASK);
}

void PORT_built_in_led_toggle(void) {
  gpio_toggle(PORT_BUILT_IN_LED, BUILT_IN_LED_MASK);
}

void PORT_buzzer_on(void) { timer_enable_oc_output(TIM4, TIM_OC4); }

void PORT_buzzer_off(void) { timer_disable_oc_output(TIM4, TIM_OC4); }
