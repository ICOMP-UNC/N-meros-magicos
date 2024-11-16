
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
#include "motor_ctrl.h"
#include "sensors.h"
#include "stdint.h"
#include "string.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/

#define COMM_UART_PORT USART1

#define MSG_INIT "1 - INICIALIZAR MOTOR\r\n"
#define MSG_CONFIG_SPEED "2 - CONFIGURAR VELOCIDAD\r\n"
#define MSG_CLOSE "3 - CERRAR PUERTA\r\n"
#define MSG_OPEN "4 - ABRIR PUERTA\r\n"
#define MSG_STOP "5 - DETENER PUERTA\r\n"
#define MSG_READ_SENSORS "6 - LEER TEMPERATURAS\r\n"

#define MSG_CONFIG_SPEED_LOW "l - VELOCIDAD BAJA\r\n"
#define MSG_CONFIG_SPEED_MED "m - VELOCIDAD MEDIA\r\n"
#define MSG_CONFIG_SPEED_HIGH "h - VELOCIDAD ALTA\r\n"
#define MSG_CONFIG_ENABLE "e - HABILITAR\r\n"
#define MSG_CONFIG_DISABLE "d - DEHABILITAR\r\n"

#define MSG_INVALID_CMD "COMANDO INVALIDO\r\n"
#define MSG_CMD_OK "COMANDO OK\r\n"
#define MSG_TEMP_ALARM "TEMPARATURA ELEVADA\r\n"
#define MSG_TEMP_OK "TEMPARATURA OK\r\n"

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/

enum CMD {
  CMD_INIT = '1',
  CMD_CONFIG_SPEED = '2',
  CMD_CLOSE = '3',
  CMD_OPEN = '4',
  CMD_STOP = '5',
  CMD_READ_SENSORS = '6',
};
enum ARGS {
  CMD_LOW_SPEED = 'l',
  CMD_MED_SPEED = 'm',
  CMD_HIGH_SPEED = 'h',
  CMD_ENABLE = 'e',
  CMD_DISABLE = 'd',
};

enum evt {
  INIT_SEQUENCE,
  CONFIG_SPEED,
  CONFIG_SENSOR_LIGHT,
  CONFIG_SENSOR_MOVE,
  CLOSE,
  OPEN,
  STOP,
};

enum {
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

static void send_data(uint8_t *data, size_t size);
static void move_fsm(void);
static void show_menu(void);
void usart1_isr(void);

/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

static void send_data(uint8_t *data, size_t size) {
  while (size) {
    usart_send_blocking(COMM_UART_PORT, *data++);
    size--;
  }
}
static void move_fsm(void) {
  if (!new_msg)
    return;

  if (byte_rcv == '\r' || byte_rcv == '\n')
    return;

  switch (current_state) {
  case WAITING_CMD:
    switch (byte_rcv) {
    case CMD_INIT:
      MOTOR_CTRL_start_routine();
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      next_state = WAITING_CMD;
      send_menu = 1;
      break;
    case CMD_CONFIG_SPEED:
      next_state = WAITING_SPEED;
      send_menu = 1;
      break;
    case CMD_CLOSE:
      MOTOR_CTRL_close();
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;
    case CMD_OPEN:
      MOTOR_CTRL_open();
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;
    case CMD_READ_SENSORS:
      next_state = WAITING_CMD;
      send_menu = 1;
      char history[256];
      uint16_t history_size;
      history_size = SENSORS_read_temp_history(history);
      send_data((uint8_t *)history, history_size);
      break;

    default:
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD);
      break;
    }
    break;
  case WAITING_SPEED:
    switch (byte_rcv) {
    case CMD_LOW_SPEED:
      MOTOR_CTRL_set_speed(MOTOR_CTRL_LOW_SPEED);
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;
    case CMD_MED_SPEED:
      MOTOR_CTRL_set_speed(MOTOR_CTRL_MED_SPEED);
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;
    case CMD_HIGH_SPEED:
      MOTOR_CTRL_set_speed(MOTOR_CTRL_HIGH_SPEED);
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;

    default:
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD);
      break;
    }

    break;
  case WAITING_LIGHT_CONFIG:
    switch (byte_rcv) {
    case CMD_ENABLE:
      // LIGHT_CTRL_set_speed(LOW)
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;
    case CMD_DISABLE:
      // LIGHT_CTRL_set_speed(LOW)
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;

    default:
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD);
      break;
    }
    break;
  case WAITING_MOVE_CONFIG:
    switch (byte_rcv) {
    case CMD_ENABLE:
      // MOVE_CTRL_set_speed(LOW)
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;
    case CMD_DISABLE:
      // MOVE_CTRL_set_speed(LOW)
      send_data((uint8_t *)MSG_CMD_OK, sizeof MSG_CMD_OK);
      break;

    default:
      next_state = WAITING_CMD;
      send_menu = 1;
      send_data((uint8_t *)MSG_INVALID_CMD, sizeof MSG_INVALID_CMD);
      break;
    }
    break;
  default:
    break;
  }
  current_state = next_state;
  new_msg = 0;
}

static void show_menu(void) {
  if (!send_menu)
    return;
  send_menu = 0;
  switch (current_state) {
  case WAITING_CMD:
    send_data((uint8_t *)MSG_INIT, sizeof MSG_INIT);
    send_data((uint8_t *)MSG_CONFIG_SPEED, sizeof MSG_CONFIG_SPEED);
    send_data((uint8_t *)MSG_CLOSE, sizeof MSG_CLOSE);
    send_data((uint8_t *)MSG_OPEN, sizeof MSG_OPEN);
    send_data((uint8_t *)MSG_STOP, sizeof MSG_STOP);
    send_data((uint8_t *)MSG_READ_SENSORS, sizeof MSG_READ_SENSORS);
    break;
  case WAITING_SPEED:
    send_data((uint8_t *)MSG_CONFIG_SPEED_LOW, sizeof MSG_CONFIG_SPEED_LOW);
    send_data((uint8_t *)MSG_CONFIG_SPEED_MED, sizeof MSG_CONFIG_SPEED_MED);
    send_data((uint8_t *)MSG_CONFIG_SPEED_HIGH, sizeof MSG_CONFIG_SPEED_HIGH);
    break;
  case WAITING_LIGHT_CONFIG:
    send_data((uint8_t *)MSG_CONFIG_DISABLE, sizeof MSG_CONFIG_DISABLE);
    send_data((uint8_t *)MSG_CONFIG_ENABLE, sizeof MSG_CONFIG_ENABLE);
    break;
  case WAITING_MOVE_CONFIG:
    send_data((uint8_t *)MSG_CONFIG_DISABLE, sizeof MSG_CONFIG_DISABLE);
    send_data((uint8_t *)MSG_CONFIG_ENABLE, sizeof MSG_CONFIG_ENABLE);
    break;
  default:
    break;
  }
}

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void COMM_UART_init(void) {
  // Enable clocks for GPIOA and USART1
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_USART1);

  // Configure USART1 TX (PA9) as alternate function push-pull
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                GPIO9);

  // Configure USART1 RX (PA10) as input floating
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO10);

  // Configure USART1 settings: baud rate, data bits, stop bits, etc.
  usart_set_baudrate(USART1, 9600);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX_RX); // Set to both TX and RX mode
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_enable_rx_interrupt(USART1);
  nvic_enable_irq(NVIC_USART1_IRQ);

  // Enable USART1
  usart_enable(USART1);
  send_menu = 1;
  current_state = WAITING_CMD;
}

void COMM_UART_loop(void) {
  move_fsm();
  show_menu();
}
void COMM_UART_temp_alarm(void) {
  send_data((uint8_t *)MSG_TEMP_ALARM, sizeof MSG_TEMP_ALARM);
}
void COMM_UART_temp_ok(void) {
  send_data((uint8_t *)MSG_TEMP_OK, sizeof MSG_TEMP_OK);
}

void usart1_isr(void) {
  if (usart_get_flag(USART1, USART_FLAG_RXNE)) { // Check if data is received
    byte_rcv = usart_recv(USART1);               // Read the received byte
    new_msg = 1;
  }
}