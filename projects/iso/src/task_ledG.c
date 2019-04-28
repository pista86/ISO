#include "task_ledG.h"

pistaOS_event_t eventoLedG_ON;
uint32_t TonG;

// Implementacion de funcion de la tarea
void * task_ledG(void* arg) {

	eventoLedG_ON = event_init();

	while (TRUE) {

		event_wait(&eventoLedG_ON);

		gpioWrite(LED3, 1);

		taskDelay(TonG);

		gpioWrite(LED3, 0);
	}

}
