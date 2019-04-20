#include "pistaOS.h"

static void schedule(void);
static void initIdleTask(void);
static int32_t get_ready_task_index(void);

static uint32_t sysTickCount;

uint32_t stackIdleTask[IDLE_STACK_SIZE_B / 4];

pistaOS_task_t taskIdle;
int32_t currentTaskIndex;
uint8_t firstRun;

void OSInit(void) {

	sysTickCount = 0;

	NVIC_SetPriority(PendSV_IRQn, (1 << 3) - 1);

	initIdleTask();

	initTasks();
}

void SysTick_Handler(void) {

	sysTickCount++;

	// Actualizar ticks de tareas con delay

	for (int i = 0; i < PISTAOS_MAX_TASK; i++) {
		if (taskList[i].status == TASK_WAITING && taskList[i].delayTicks > 0) {
			if ((--taskList[i].delayTicks) == 0) {
				taskList[i].status = TASK_READY;
			}
		}
	}

	// Llamo a PendSV
	schedule();
}

void taskDelay(uint32_t delayTicks) {
	// Configuro delays y pongo tarea en modo WAITING
	taskList[get_running_task_index()].delayTicks = delayTicks;
	taskList[get_running_task_index()].status = TASK_WAITING;

	// Llamo a PendSV
	schedule();
}


static void initIdleTask(void) {

	// Seteo de valores de configuración iniciail para task2
	taskIdle.arg = 0;
	taskIdle.entry_point = idleTask;
	taskIdle.return_point = idleTask;
	taskIdle.stack_size_bytes = IDLE_STACK_SIZE_B;
	taskIdle.stack = stackIdleTask;

	// Inicialización de tarea idle
	bzero(taskIdle.stack, taskIdle.stack_size_bytes);

	taskIdle.stack[taskIdle.stack_size_bytes / 4 - 1] = 1 << 24; // xPSR.T = 1
	taskIdle.stack[taskIdle.stack_size_bytes / 4 - 2] =
			(uint32_t) taskIdle.entry_point; // PC
	taskIdle.stack[taskIdle.stack_size_bytes / 4 - 3] =
			(uint32_t) taskIdle.return_point; // LR
	taskIdle.stack[taskIdle.stack_size_bytes / 4 - 8] = (uint32_t) taskIdle.arg; // R0
	taskIdle.stack[taskIdle.stack_size_bytes / 4 - 9] = 0xFFFFFFF9;

	taskIdle.sp =
			(uint32_t) &(taskIdle.stack[taskIdle.stack_size_bytes / 4 - 17]); // considero los otros 8 registros

	currentTaskIndex = -1;
	firstRun = 1;
}

void taskInit(pistaOS_task_t task, uint8_t taskNumber) {

	if (taskNumber >= 0) {

		bzero(task.stack, task.stack_size_bytes);

		task.stack[task.stack_size_bytes / 4 - 1] = 1 << 24; // xPSR.T = 1
		task.stack[task.stack_size_bytes / 4 - 2] = (uint32_t) task.entry_point; // PC
		task.stack[task.stack_size_bytes / 4 - 3] =
				(uint32_t) task.return_point; // LR
		task.stack[task.stack_size_bytes / 4 - 8] = (uint32_t) task.arg; // R0
		task.stack[task.stack_size_bytes / 4 - 9] = 0xFFFFFFF9;

		task.sp = (uint32_t) &(task.stack[task.stack_size_bytes / 4 - 17]); // considero los otros 8 registros

		task.status = TASK_READY;

		taskList[taskNumber] = task;
	}

}

uint32_t get_next_context(uint32_t current_sp) {
	uint32_t next_sp;
	int32_t ready_task_index;

	ready_task_index = get_ready_task_index(); // Buscar tarea READY de mayor prioridad

	if (ready_task_index != currentTaskIndex) // Tarea READY de mayor prioridad es distinta a la que se ejecuta?
			{
		//hay que cambiar de contexto

		if (currentTaskIndex == -1) {
			if (firstRun == 0) {
				taskIdle.sp = current_sp; // La tarea en ejecución era la idle, guardo contexto
			}
			firstRun = 0;
		} else {
			taskList[currentTaskIndex].sp = current_sp; // La tarea en ejecución era una definida por el usuario, guardo contexto

			if (taskList[currentTaskIndex].status == TASK_RUNNING) {
				/* Si la tarea anterior estaba corriendo pasa a ready
				 * si fue suspendida por un taskDelay ya está en waiting y no se cambia el estado
				 * si fue suspendida por evento ya está en SUSPENDED y no se cambia el estado */
				taskList[currentTaskIndex].status = TASK_READY;
			}
		}

		if (ready_task_index == -1) {

			next_sp = taskIdle.sp; // no hay tareas READY definidas por usuario, nuevo contexto es task idle
			currentTaskIndex = -1;
		} else {
			// nuevo contexto es otra tarea

			next_sp = taskList[ready_task_index].sp;
			currentTaskIndex = ready_task_index;
			taskList[currentTaskIndex].status = TASK_RUNNING;

		}

	} else {
		next_sp = current_sp; // no hay cambio de contexto
	}

	return next_sp;
}

void set_task_status(uint32_t taskIndex, pistaOS_task_status_t status) {

	taskList[taskIndex].status = status;

	schedule();
}

uint32_t get_running_task_index(void) {

	uint32_t runningTaskIndex = 0;

	for (int i = 0; i < PISTAOS_MAX_TASK; i++) {
		if (taskList[i].status == TASK_RUNNING) {
			runningTaskIndex = i;
			break;
		}
	}

	return runningTaskIndex;
}

uint32_t get_sysTickCount(void) {
	return sysTickCount;
}

static int32_t get_ready_task_index(void) {

	uint32_t maxPriority = 0;
	int32_t readyTaskIndex = -1;

	for (int i = 0; i < PISTAOS_MAX_TASK; i++) {
		// Buscar dentro de la listas de tareas la READY de mayor prioridad
		if (taskList[i].status == TASK_READY
				&& taskList[i].priority >= maxPriority) {

			readyTaskIndex = i;
			maxPriority = taskList[i].priority;

		}

	}

	return readyTaskIndex;

}

static void schedule(void) {

	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	__ISB();

	__DSB();

}
