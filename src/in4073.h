/*------------------------------------------------------------------
 *  in4073.h -- defines, globals, function prototypes
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  July 2016
 *------------------------------------------------------------------
 */

#ifndef IN4073_H__
#define IN4073_H__

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include "comm_protocol.h"
#include "comm_drone.h"
#include "logger.h"
#include "states.h"

#ifndef TESTING
 #include "nrf_gpio.h"
 #include "nrf_delay.h"
 #include "inv_mpu.h"
 #include "inv_mpu_dmp_motion_driver.h"
 #include "ml.h"
 #include "app_util_platform.h"
#endif

#define RED		    22
#define YELLOW		24
#define GREEN		28
#define BLUE		30
#define INT_PIN		5

// Control
int16_t motor[4];
int32_t ae[4];

// Timers
#define TIMER_PERIOD	50 //50ms=20Hz (MAX 23bit, 4.6h)
void timers_init(void);
uint32_t get_time_us(void);
bool check_timer_flag(void);
void clear_timer_flag(void);

// GPIO
void gpio_init(void);

// Queue
#define QUEUE_SIZE 256
typedef struct {
  uint8_t Data[QUEUE_SIZE];
  uint16_t first,last;
  uint16_t count; 
} queue;
void init_queue(queue *q);
void enqueue(queue *q, char x);
char dequeue(queue *q);

// UART
#define RX_PIN_NUMBER  16
#define TX_PIN_NUMBER  14
queue rx_queue;
queue tx_queue;
uint32_t last_correct_checksum_time;
void uart_init(void);
void uart_put(uint8_t);

// TWI
#define TWI_SCL	4
#define TWI_SDA	2
void twi_init(void);
bool i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t const *data);
bool i2c_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);

// MPU wrapper
int16_t phi, theta, psi;
int16_t sp, sq, sr;

int16_t phi_offset, theta_offset, psi_offset; // Defining the calibration offsets, so they are global!
int16_t sp_offset, sq_offset, sr_offset;

int16_t sax, say, saz;
uint8_t sensor_fifo_count;
void imu_init(bool dmp, uint16_t interrupt_frequency); // if dmp is true, the interrupt frequency is 100Hz - otherwise 32Hz-8kHz
void get_dmp_data(void);
void get_raw_sensor_data(void);
bool check_sensor_int_flag(void);
void clear_sensor_int_flag(void);

// Barometer
int32_t pressure;
int32_t temperature;
void read_baro(void);
void baro_init(void);

// ADC
#ifdef DRONE
  #define BAT_WARN 1100
  #define BAT_MIN 1050
#else
  #define BAT_WARN 100
  #define BAT_MIN 0
#endif

uint16_t bat_volt;
void adc_init(void);
void adc_request_sample(void);

// Flash
bool spi_flash_init(void);
bool flash_chip_erase(void);
bool flash_write_byte(uint32_t address, uint8_t data);
bool flash_write_bytes(uint32_t address, uint8_t *data, uint32_t count);
bool flash_read_byte(uint32_t address, uint8_t *buffer);
bool flash_read_bytes(uint32_t address, uint8_t *buffer, uint32_t count);

// BLE
queue ble_rx_queue;
queue ble_tx_queue;
volatile bool radio_active;
void ble_init(void);
void ble_send(void);

// Counters
uint16_t led_counter;
uint16_t info_msg_counter;

#endif // IN4073_H__
