#include "task_ledY.h"

pistaOS_event_t eventoLedY_ON;
uint32_t TonY;

// Implementacion de funcion de la tarea
void * task_ledY(void* arg) {

	eventoLedY_ON = event_init();

	while (TRUE) {

		event_wait(&eventoLedY_ON);

		gpioWrite(LED1, 1);

		taskDelay(TonY);

		gpioWrite(LED1, 0);
	}

}
