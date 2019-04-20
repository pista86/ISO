/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "pistaOS.h"
#include "task_debounce.h"
#include "task_uart.h"
#include "task2.h"

/*==================[macros and definitions]=================================*/

#define STACK_SIZE_B 512


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
uint32_t stack3[STACK_SIZE_B / 4];

pistaOS_task_t taskDebounce;
pistaOS_task_t taskUART;
pistaOS_task_t task3;

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

	// Seteo de valores de configuración iniciail para task_debounce
	taskUART.arg = 0;
	taskUART.priority = 4;
	taskUART.entry_point = uart_task;
	taskUART.stack_size_bytes = STACK_SIZE_B;
	taskUART.stack = stackUART;

	// Seteo de valores de configuración iniciail para task2
	task3.arg = 0;
	task3.priority = 3;
	task3.entry_point = taskMain2;
	task3.stack_size_bytes = STACK_SIZE_B;
	task3.stack = stack3;

	// Inicialización de tareas
	taskInit(taskDebounce, 0);
	taskInit(taskUART, 1);
	taskInit(task3, 2);
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
