/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "pistaOS.h"
#include "task_debounce.h"
#include "task_uart.h"
#include "task_ledR.h"
#include "task_ledG.h"
#include "task_ledB.h"
#include "task_ledY.h"

/*==================[macros and definitions]=================================*/

#define STACK_SIZE_B 1024

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);
static void inicializarTareas(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

uint32_t stackDebounce[STACK_SIZE_B / 4];
uint32_t stackUART[STACK_SIZE_B / 4];
uint32_t stackLedR[STACK_SIZE_B / 4];
uint32_t stackLedG[STACK_SIZE_B / 4];
uint32_t stackLedB[STACK_SIZE_B / 4];
uint32_t stackLedY[STACK_SIZE_B / 4];

pistaOS_task_t taskDebounce;
pistaOS_task_t taskUART;
pistaOS_task_t taskLedR;
pistaOS_task_t taskLedG;
pistaOS_task_t taskLedB;
pistaOS_task_t taskLedY;

/*==================[internal functions definition]==========================*/

static void initHardware(void) {
	Board_Init();
	boardConfig();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

/*==================[external functions definition]==========================*/

void task_return_hook(void * ret_val) {
	while (1) {
		__WFI();
	}
}

int main(void) {

	// inicialización del hardware
	initHardware();

	OSInit();

	// Inicialización de tareas
	inicializarTareas();

	while (1) {
		__WFI();
	}
}

static void inicializarTareas(void) {

	// Seteo de valores de configuración iniciail para task_debounce
	taskDebounce.arg = 0;
	taskDebounce.priority = 5;
	taskDebounce.entry_point = task_debounce;
	taskDebounce.stack_size_bytes = STACK_SIZE_B;
	taskDebounce.stack = stackDebounce;

	// Seteo de valores de configuración iniciail para uart_task
	taskUART.arg = 0;
	taskUART.priority = 4;
	taskUART.entry_point = uart_task;
	taskUART.stack_size_bytes = STACK_SIZE_B;
	taskUART.stack = stackUART;

	// Seteo de valores de configuración iniciail para taskLedR
	taskLedR.arg = 0;
	taskLedR.priority = 6;
	taskLedR.entry_point = task_ledR;
	taskLedR.stack_size_bytes = STACK_SIZE_B;
	taskLedR.stack = stackLedR;

	// Seteo de valores de configuración iniciail para taskLedG
	taskLedG.arg = 0;
	taskLedG.priority = 7;
	taskLedG.entry_point = task_ledG;
	taskLedG.stack_size_bytes = STACK_SIZE_B;
	taskLedG.stack = stackLedG;

	// Seteo de valores de configuración iniciail para taskLedB
	taskLedB.arg = 0;
	taskLedB.priority = 8;
	taskLedB.entry_point = task_ledB;
	taskLedB.stack_size_bytes = STACK_SIZE_B;
	taskLedB.stack = stackLedB;

	// Seteo de valores de configuración iniciail para taskLedY
	taskLedY.arg = 0;
	taskLedY.priority = 9;
	taskLedY.entry_point = task_ledY;
	taskLedY.stack_size_bytes = STACK_SIZE_B;
	taskLedY.stack = stackLedY;

	// Inicialización de tareas
	taskInit(taskDebounce, 0);
	taskInit(taskUART, 1);
	taskInit(taskLedR, 3);
	taskInit(taskLedG, 4);
	taskInit(taskLedB, 5);
	taskInit(taskLedY, 6);
}

/*==================[end of file]============================================*/
