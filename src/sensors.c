/*
 * sensors.c
 *
 *  Created on: Jun 14, 2024
 *      Author: Horacio
 */

/********************************************************************
 *                      INCLUSIONES
 ********************************************************************/
#include "sensors.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include "port.h"
#include "stdio.h"
#include "motor_ctrl.h"
#include "output.h"
/********************************************************************
 *                      DEFINICIONES
 ********************************************************************/
#define ADC_CHANNEL_TEMP_SENSOR ADC_CHANNEL0 

#define MAX_TEMP  40

#define ADC_BUFFER_SIZE 5 /* Buffer size for averaging */
#define HISTORY_SIZE 5
#define TRUE 1

/********************************************************************
 *                      ENUMERADOS
 ********************************************************************/


/********************************************************************
 *                      STRUCTS
 ********************************************************************/


/********************************************************************
 *                      VARIABLES LOCALES
 ********************************************************************/

static uint8_t head;
static uint8_t tail;
static uint8_t size;

uint16_t adc_buffer[ADC_BUFFER_SIZE];
float temp_history[HISTORY_SIZE];

/********************************************************************
 *                      PROTOTIPO FUNCIONES LOCALES
 ********************************************************************/
void adc_init(void);
void timer2_init(void);
void dma_init(void);

/********************************************************************
 *                      FUNCIONES LOCALES
 ********************************************************************/

void adc_init(void)
{
    /* Enable ADC1 clock */
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Configure ADC1 */
    adc_power_off(ADC1);
    adc_disable_scan_mode(ADC1);
    adc_set_continuous_conversion_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_sample_time(ADC1, ADC_CHANNEL_TEMP_SENSOR, ADC_SMPR_SMP_55DOT5CYC); /* Set sample time */

    /* Calibrate ADC1 */
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate(ADC1);
}

void timer2_init(void)
{
    /* Enable Timer 2 clock */
    rcc_periph_clock_enable(RCC_TIM2);

    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM2_IRQ);

    /* Reset TIM2 peripheral to defaults. */
    rcc_periph_reset_pulse(RST_TIM2);

    /* Set timer prescaler for 1Hz (1 tick per second) */
    timer_set_prescaler(TIM2, 7200 - 1);
    timer_set_period(TIM2, 50000 - 1);

    /* Enable Timer 2 interrupt for periodic triggering */
    timer_enable_irq(TIM2, TIM_DIER_UIE);

    /* Start the timer */
    timer_enable_counter(TIM2);
}

void dma_init(void)
{
    /* Enable DMA1 clock */
    rcc_periph_clock_enable(RCC_DMA1);

    /* Reset DMA1 stream */
    dma_channel_reset(DMA1, DMA_CHANNEL1);

    /* Set DMA configuration */
    dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)&ADC_DR(ADC1)); /* ADC data register */
    dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)adc_buffer);        /* Memory buffer */
    dma_set_number_of_data(DMA1, DMA_CHANNEL1, ADC_BUFFER_SIZE);             /* Number of data items */
    dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_LOW);
    dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);     /* Memory size: 16 bits */
    dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT); /* Peripheral size: 16 bits */
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL1); /* Enable circular mode */

    /* Start DMA transfer */
    dma_enable_channel(DMA1, DMA_CHANNEL1);

    /* Enable ADC DMA mode */
    adc_enable_dma(ADC1);

    adc_start_conversion_direct(ADC1);
}


float get_temperature(uint16_t adc_read)
{
    float ret =  ((float)adc_read * 330.0 / 4096.0);
    return ret;
}

uint16_t average_adc_value(void)
{
    uint32_t sum = 0;
    for (int i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        sum += adc_buffer[i];
    }
    return (uint16_t)(sum / ADC_BUFFER_SIZE);
}

void tim2_isr(void)
{
    if (timer_get_flag(TIM2, TIM_SR_UIF))
    {
        timer_clear_flag(TIM2, TIM_SR_UIF); // Clear update interrupt flag

        float temp = get_temperature(average_adc_value());
        if (temp > MAX_TEMP)
        {
            OUTPUT_buzzer_on();
            MOTOR_CTRL_close();
        }

        if (temp < MAX_TEMP)
            OUTPUT_buzzer_off();

        temp_history[head] = temp;

        if (size < HISTORY_SIZE)
            size++;
        else
            tail++;

        head++;
        head = head % HISTORY_SIZE;
        tail = tail % HISTORY_SIZE;

    }
}

/********************************************************************
 *                      FUNCIONES GLOBALES
 ********************************************************************/

void SENSORS_init(void)
{
    adc_init();
    dma_init();
    timer2_init();
}

uint16_t SENSORS_read_temp_history(char *buffer)
{
    uint8_t size_aux = size;
    uint8_t tail_aux = tail;
    size_t len = 0;
    int8_t pos;
    float temp;
    for (uint8_t i = 0; i < size_aux; i++)
    {
        pos = tail_aux + i;
        pos = pos % HISTORY_SIZE;

        temp = temp_history[pos];
        len += sprintf(buffer + len, "%d.%d°, ", (int32_t)temp, (int32_t)(temp*10)%10) - 1;
    }
    buffer[len] = '\n';
    len++;
    buffer[len] = 0;
    return len;
    
}