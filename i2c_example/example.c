/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 * Copyright (c) 2022 Davide Del Vento
 *
 * SPDX-License-Identifier: MIT
 */

#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hardware/irq.h>
#include "hardware/i2c.h"
#include "pico/multicore.h"

static const uint I2C_SLAVE_ADDRESS = 0x74;
static const uint I2C_BAUDRATE = 100000;

// For this example, we run both the master and slave from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_SLAVE_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // 4
static const uint I2C_SLAVE_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // 5
static const uint I2C_MASTER_SDA_PIN = 6;
static const uint I2C_MASTER_SCL_PIN = 7;

#define SIZE 4

void worker_sending() {
    uint8_t buffer[SIZE];
    for(int i=0; i<SIZE; i++) {
	    buffer[i] = i;
    }
    i2c_write_raw_blocking(i2c0, buffer, SIZE);
}

void other_core() {
    while (true) {
	// sleep_ms(2500);                             // this works 
        printf("Worker about to send data\n");
	worker_sending();
	printf("Worker sent data\n");
	sleep_ms(2500);                                // this does not work
    }
}

int main() {
    stdio_init_all();

    sleep_ms(4000);                  // time to start minicom on computer
    printf("pico I2C controller/worker example\n");

    // worker side

    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);

    i2c_set_slave_mode(i2c0, true, I2C_SLAVE_ADDRESS);
//    irq_set_exclusive_handler(I2C0_IRQ, &worker_sending);
//    irq_set_enabled(I2C0_IRQ, true);
    multicore_launch_core1(other_core);
    

    // end worker side


    // controller side

    gpio_init(I2C_MASTER_SDA_PIN);
    gpio_set_function(I2C_MASTER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_MASTER_SDA_PIN);

    gpio_init(I2C_MASTER_SCL_PIN);
    gpio_set_function(I2C_MASTER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_MASTER_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);

    uint8_t buffer[SIZE];
    while (true) {
        for(int i=0; i<SIZE; i++) {
	    buffer[i] = 0;
        }
	printf("This is what was I have: ");
	for (int i=0; i<SIZE; i++) {
	    printf("%d ", buffer[i]);
	}
	printf("\n");

        i2c_read_blocking(i2c1, I2C_SLAVE_ADDRESS, buffer, SIZE, false);

	printf("This is what I received: ");
	for (int i=0; i<SIZE; i++) {
	    printf("%d ", buffer[i]);
	}
	printf("\n");
	sleep_ms(2000);
    }

}
