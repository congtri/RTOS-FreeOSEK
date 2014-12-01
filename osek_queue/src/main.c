/*
 ===============================================================================
 Name        : osek_example_app.c
 Author      : Pablo Ridolfi
 Version     :
 Copyright   : (c)2014, Pablo Ridolfi, DPLab@UTN-FRBA
 Description : main definition
 ===============================================================================
 */

#include "board.h"

#include <stdio.h>
#include <cr_section_macros.h>

#include "os.h"               /* <= operating system header */

#include <queue.h>

static queue_t queue;
static int counter;

//#define printf(...)
int main(void) {
	SystemCoreClockUpdate();

	Board_Init();

	Board_LED_Set(0, false);
//    Chip_GPIO_SetPinDIR(LPC_GPIO, 2, 1, 1);
//    Chip_GPIO_SetPinDIR(LPC_GPIO, 2, 0, 1);
//    Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 26, 1);


	queue_init(&queue, eventQueue);

	counter = 0;

	StartOS(AppMode1);

	while (1)
		;

	return 0;
}

TASK(taskToggle) {
	Board_LED_Toggle(0);
	TerminateTask();
}

TASK(taskProducer) {
	counter++;
	printf("Task Producer: pushing value %d\n", counter);
	queue_push(&queue, counter);
	TerminateTask();
}

TASK(taskConsumer) {
	static int value;

	queue_pop(&queue, &value);

	printf("Task Consumer: popped value %d\n", value);

	TerminateTask();
}

void ErrorHook(void) {
	/* kernel panic :( */
	ShutdownOS(0);
}