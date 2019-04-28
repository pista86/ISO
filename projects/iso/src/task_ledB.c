#include "task_ledB.h"

pistaOS_event_t eventoLedB_ON;
uint32_t TonB;

// Implementacion de funcion de la tarea
void * task_ledB(void* arg) {

	eventoLedB_ON = event_init();

	while (TRUE) {

		event_wait(&eventoLedB_ON);

		gpioWrite(LEDB, 1);

		taskDelay(TonB);

		gpioWrite(LEDB, 0);
	}

}
