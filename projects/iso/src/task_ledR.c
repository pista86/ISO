#include "task_ledR.h"

pistaOS_event_t eventoLedR_ON;
uint32_t TonR;

// Implementacion de funcion de la tarea
void * task_ledR(void* arg) {

	eventoLedR_ON = event_init();

	while (TRUE) {

		event_wait(&eventoLedR_ON);

		gpioWrite(LED2, 1);

		taskDelay(TonR);

		gpioWrite(LED2, 0);
	}

}
