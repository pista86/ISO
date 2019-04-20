#include "task2.h"
#include "task_uart.h"

void * taskMain2(void * arg) {



	mensajeParaTx.buffer = "123456789 \n";
	mensajeParaTx.longitud = 11;


	while (1) {
		__WFI();


		event_set(eventoUartTX_full);





		taskDelay(1);


	}

	return NULL;
}
