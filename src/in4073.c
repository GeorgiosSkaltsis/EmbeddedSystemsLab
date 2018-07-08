/*------------------------------------------------------------------
 *  in4073.c -- test QR engines and sensors
 *
 *  reads ae[0-3] uart rx queue
 *  (q,w,e,r increment, a,s,d,f decrement)
 *
 *  prints timestamp, ae[0-3], sensors to uart tx queue
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  June 2016
 *------------------------------------------------------------------
 */

#include "in4073.h"
#include <stdlib.h>
#include "filters.h"
#include <string.h>

uint32_t start, end;
uint32_t time_accumulated = 0;

/**
 * Initialize the drone.
 * - Setup sensors
 * - Setup data connections
 * 
 * Author: Chiel Bruin
 */
void init_drone()
{
	uart_init();
	gpio_init();
	timers_init();
	adc_init();
	twi_init();
	imu_init(true, 100);	
	baro_init();
	spi_flash_init();
	ble_init();
	logger_init();
	comm_protocol_init();
	filter_init();

}

/**
 * Shutdown the drone.
 * 
 * Author: Chiel Bruin
 */
void drone_shutdown()
{
	printf("\n\t Goodbye \n\n");
	nrf_delay_ms(100);
	NVIC_SystemReset();
}

/**
 * Read the data from the serial interface.
 * 
 * Author: Chiel Bruin
 */
void read_serial()
{
	while (rx_queue.count) {
		char c = dequeue(&rx_queue);
		byte_enqueue(&rcv_queue, c);
	}
}

/**
 * Run all the events that run on sometimes.
 * In this case, just blink the LED.
 * 
 * Author: Chiel Bruin
 */
void run_sporadic_events()
{	
	if (check_timer_flag()) {
		// Decrement timeout time, we go to panic mode on timeout
		if (--timeout_counter == 0 && current_mode != safe_mode) {
			incoming_control_mode = panic_mode;
		}

		// Receive some frame data
		comm_protocol_update_rcv();

		// Blink LED
		if (led_counter % 20 == 0) {
			led_counter = 0;
			nrf_gpio_pin_toggle(BLUE);
		}
		led_counter++;
	}
}

/**
 * Check if the timer can be reset, so we can have multiple elements
 * depend on the timer flag.
 * 
 * Author: M. van de Ruit
 */
void check_timer()
{
	if (check_timer_flag()) 
	{
		clear_timer_flag();
	}
}

/**
 * Log the relevant data for this iteration.
 * 
 * Author: Chiel Bruin, M. van de Ruit
 */
uint32_t test_int = 0;
void log_data()
{	
	if (info_msg_counter % 10000 == 0) {
		// Store a time log
		char time_log_space[28];
		sprintf(time_log_space, "10K iterations took %6ldus", time_accumulated / 10000);
		log_message(time_log_space, 28);
		time_accumulated = 0;
	}

	if (info_msg_counter % 1000 == 0) {
		// Send an info log back
		char info_space[info_size];
		sprintf(info_space, "%10lu | %3d %3d %3d %3d | %6d %6d %6d | %6d %6d %6d | %4d | %4ld | %6ld | %3d %3d %3d %3d | %1d",
			get_time_us(),
			// incoming_pitch, incoming_roll, incoming_yaw, incoming_lift,
			motor[0], motor[1], motor[2], motor[3],
			phi - phi_offset, theta - theta_offset, psi- psi_offset,
			sp - sp_offset, sq - sq_offset, sr - sr_offset,
			bat_volt,temperature, pressure,
			incoming_k_y, incoming_k_r_1, incoming_k_r_2, incoming_par_3,
			current_mode
		);
		snd_info(info_space);
	}
	
	if (info_msg_counter % 1000 == 0) {
		// Send battery warning if needed
		if (bat_volt < BAT_WARN) {
			snd_battery_low();
		}
	}

	info_msg_counter++;
}

/**
 * Read all the new sensor data (when available).
 * Returns true when new data was received, false otherwise.
 * 
 * Author: Chiel Bruin
 * Edited: Dhaval Shah. (Added the case for Raw Mode)
 */
int read_sensors()
{
	int8_t new_data = 0;
	
	// Read timed sensors
	if (check_timer_flag()) 
	{
		adc_request_sample();
		read_baro();
		new_data = true; // This should be turne to false if wanna get raw data I think!!!!!
	}

	// Read interrupting sensors
	if (check_sensor_int_flag() && current_mode != raw_mode) 
	{
		get_dmp_data();
		new_data = true;
	}
    
	if (check_sensor_int_flag() && current_mode == raw_mode) 
	{
        get_raw_sensor_data();
		new_data = true;	
	}
		
	return new_data;
}

/**
 * Main method, runs the main loop of the drone.
 * The loop makes use of an implicit state machine containing 7 states.
 * 
 * Author: Chiel Bruin
 */
int main(void)
{ 	
	// STATE 0: Startup
	init_drone();
	log_message("Starting up", 11);

	while (!should_abort_or_exit)
	{
		// Time at start of loop
		start = get_time_us();

		// STATE 1: Read serial data
		read_serial();
		
		// STATE 2: Read sensor data
		if (read_sensors() && current_mode == raw_mode)
		{
			// STATE 3: Process sensor data
			filter_data();
		}

		// STATE 4: Update engines
		mode_step(incoming_control_mode);
		
		// STATE 5: Log data
		log_data();

		// STATE 6: run timed non-sensor events
		run_sporadic_events();

		// STATE 7: possibly reset timer
		check_timer();

		// Time at end of loop
		end = get_time_us();
		time_accumulated += (end - start);
	}	

	// STATE 8: Go to safe state before exit
	mode_step(safe_mode);

	// STATE 9: Send logs to pc
	log_message("Shutting down", 13);
	flush_logs();
	
	// STATE 10: Shutdown
	drone_shutdown();
}
