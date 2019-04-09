#include "pistaOS_task.h"

void * idleTask(void * arg) {

	while (1) {
		__WFI();
	}

}

void initTasks(void)
{
	// Las tareas se inicializan en estado SUSPENDED

	for (int i = 0; i < PISTAOS_MAX_TASK; i++)
	{
		taskList[i].status = SUSPENDED;
	}

}

