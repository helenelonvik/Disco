/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <drivers/pwm.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <string.h>
#include <sys/__assert.h>
#include <kernel.h>

#define BUZZER_MIN_FREQUENCY		CONFIG_UI_BUZZER_MIN_FREQUENCY
#define BUZZER_MAX_FREQUENCY		CONFIG_UI_BUZZER_MAX_FREQUENCY
#define BUZZER_MIN_INTENSITY		0
#define BUZZER_MAX_INTENSITY		100
#define BUZZER_MIN_DUTY_CYCLE_DIV	100
#define BUZZER_MAX_DUTY_CYCLE_DIV	2

/* for thread */
#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)	// red
#define LED1_NODE DT_ALIAS(led1)	// Green
#define LED2_NODE DT_ALIAS(led2)	// Blue
#define LED29_NODE DT_ALIAS(led3)	// LIGHTWELL_RED
#define LED30_NODE DT_ALIAS(led4)	// LIGHTWELL_GREEN
#define LED31_NODE DT_ALIAS(led5)	// LIGHTWELL_BLUE

// buzzer P0.28

#if DT_NODE_HAS_STATUS(LED0_NODE, okay) // and DT_NODE_HAS_STATUS(LED1_NODE, okay) and DT_NODE_HAS_STATUS(LED2_NODE, okay)
#define LED0 DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED1 DT_GPIO_LABEL(LED1_NODE, gpios)
#define LED2 DT_GPIO_LABEL(LED2_NODE, gpios)
#define LED29 DT_GPIO_LABEL(LED29_NODE, gpios)
#define LED30 DT_GPIO_LABEL(LED30_NODE, gpios)
#define LED31 DT_GPIO_LABEL(LED31_NODE, gpios)
#define PIN0 DT_GPIO_PIN(LED0_NODE, gpios)
#define PIN1 DT_GPIO_PIN(LED1_NODE, gpios)
#define PIN2 DT_GPIO_PIN(LED2_NODE, gpios)
#define PIN29 DT_GPIO_PIN(LED29_NODE, gpios)
#define PIN30 DT_GPIO_PIN(LED30_NODE, gpios)
#define PIN31 DT_GPIO_PIN(LED31_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0 ""
#define LED1 ""
#define LED2 ""
#define LED29 ""
#define LED30 ""
#define LED31 ""
#define PIN0 0
#define PIN1 0
#define PIN2 0
#define PIN29 0
#define PIN30 0
#define PIN31 0
#define FLAGS 0
#endif

const struct device *pwm_dev;
static atomic_t buzzer_enabled;

static uint32_t intensity_to_duty_cycle_divisor(uint8_t intensity)
{
	return MIN(
		MAX(((intensity - BUZZER_MIN_INTENSITY) *
		    (BUZZER_MAX_DUTY_CYCLE_DIV - BUZZER_MIN_DUTY_CYCLE_DIV) /
		    (BUZZER_MAX_INTENSITY - BUZZER_MIN_INTENSITY) +
		    BUZZER_MIN_DUTY_CYCLE_DIV),
		    BUZZER_MAX_DUTY_CYCLE_DIV),
		BUZZER_MIN_DUTY_CYCLE_DIV);
}

static int buzzer_enable(void)
{
	int err = 0;

	atomic_set(&buzzer_enabled, 1);

#ifdef CONFIG_PM_DEVICE
	err = pm_device_state_set(pwm_dev,
				 PM_DEVICE_STATE_ACTIVE,
				 NULL, NULL);
	if (err) {
		LOG_ERR("PWM enable failed");
		return err;
	}
#endif
	return err;
}
int ui_buzzer_init(void)
{
	const char *dev_name = CONFIG_UI_BUZZER_PWM_DEV_NAME;
	int err = 0;
	pwm_dev = device_get_binding(dev_name);
	if (!pwm_dev) {
		// LOG_ERR("Could not bind to device %s", log_strdup(dev_name));
		err = -ENODEV;
	}

	buzzer_enable();
	return err;
}

static int pwm_out(uint32_t frequency, uint8_t intensity)
{
	static uint32_t prev_period;
	uint32_t period = (frequency > 0) ? USEC_PER_SEC / frequency : 0;
	uint32_t duty_cycle = (intensity == 0) ? 0 :
		period / intensity_to_duty_cycle_divisor(intensity);

	/* Applying workaround due to limitations in PWM driver that doesn't
	 * allow changing period while PWM is running. Setting pulse to 0
	 * disables the PWM, but not before the current period is finished.
	 */
	if (prev_period) {
		pwm_pin_set_usec(pwm_dev, CONFIG_UI_BUZZER_PIN,
				 prev_period, 0, 0);
		k_sleep(K_MSEC(MAX((prev_period / USEC_PER_MSEC), 1)));
	}

	prev_period = period;

	return pwm_pin_set_usec(pwm_dev, CONFIG_UI_BUZZER_PIN,
				period, duty_cycle, 0);
}

// Trying to make take on me. not working yet
// https://create.arduino.cc/projecthub/GeneralSpud/passive-buzzer-song-take-on-me-by-a-ha-0f04a8

int melody[] = {
  740, 740, 587, 494, 494, 659, 
  659, 659, 831, 831, 880, 988, 
  880, 880, 880, 659, 587, 740, 
  740, 740, 659, 659, 740, 659
};

// The note duration, 8 = 8th note, 4 = quarter note, etc.
int durations[] = {
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8, 
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8
};
// determine the length of the arrays to use in the loop iteration
int songLength = sizeof(melody)/sizeof(melody[0]);
void setup() {
 //We don't need anything here
}

void music() {
	ui_buzzer_init();
  // Iterate through both arrays
  // Notice how the iteration variable thisNote is created in the parenthesis
  // The for loop stops when it is equal to the size of the melody array
  int note = 0;
  while(1){
	printf("%d", note);
    // determine the duration of the notes that the computer understands
    // divide 1000 by the value, so the first note lasts for 1000/8 milliseconds
    int duration = 1000/ durations[note];
	pwm_out(melody[note],10);
    // pause between notes
    k_sleep(K_MSEC(duration));
	note ++;
	if (note>songLength) {
		note = 0;
	}
  }
}

void light() {
	const struct device *dev;
	int ret;
	int ret1;
	int ret2;
	int count = 0;

	dev = device_get_binding(LED0);
	if (dev == NULL)
	{
		return;
	}
	
	ret = gpio_pin_configure(dev, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS);
	ret1 = gpio_pin_configure(dev, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS);
	ret2 = gpio_pin_configure(dev, PIN2, GPIO_OUTPUT_ACTIVE | FLAGS);
	// ret29 = gpio_pin_configure(dev, PIN29, GPIO_OUTPUT_ACTIVE | FLAGS);
	// ret30 = gpio_pin_configure(dev, PIN30, GPIO_OUTPUT_ACTIVE | FLAGS);
	// ret31 = gpio_pin_configure(dev, PIN31, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0 || ret1 <0 || ret2 < 0)
	{
		return;
	}

	while (1)
	{
		if (count == 0) {
			gpio_pin_set(dev, PIN0, 1);
			gpio_pin_set(dev, PIN1, 0);
			gpio_pin_set(dev, PIN2, 0);
			count++;
		} else if (count == 1) {
			gpio_pin_set(dev, PIN0, 0);
			gpio_pin_set(dev, PIN1, 1);
			gpio_pin_set(dev, PIN2, 0);
			count++;
		} else {
			gpio_pin_set(dev, PIN0, 0);
			gpio_pin_set(dev, PIN1, 0);
			gpio_pin_set(dev, PIN2, 1);
			printk("DISCO \n");
			count = 0;
		}
		k_msleep(SLEEP_TIME_MS);
	}
}


K_THREAD_DEFINE(light_thread, MY_STACK_SIZE,
                light, NULL, NULL, NULL,
                MY_PRIORITY, 0, 0);

				
K_THREAD_DEFINE(music_thread, MY_STACK_SIZE,
                music, NULL, NULL, NULL,
                MY_PRIORITY, 0, 0);
