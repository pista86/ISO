/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "pistaOS.h"

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

uint32_t stack1[STACK_SIZE_B / 4];
uint32_t stack2[STACK_SIZE_B / 4];

pistaOS_task_t task1;
pistaOS_task_t task2;

/*==================[internal functions definition]==========================*/

static void initHardware(void) {
	Board_Init();
	boardConfig();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

/*==================[external functions definition]==========================*/

void * taskMain1(void * arg) {

	gpioToggle(LED1);
	while (1) {
		__WFI();

		gpioToggle(LED1);

		taskDelay(500);
	}

	return NULL;
}

void * taskMain2(void * arg) {

	gpioToggle(LED2);
	while (1) {
		__WFI();

		gpioToggle(LED2);

		taskDelay(4000);

	}

	return NULL;
}

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

	// Seteo de valores de configuración iniciail para task1
	task1.arg = 0;
	task1.priority = 5;
	task1.entry_point = taskMain1;
	task1.stack_size_bytes = STACK_SIZE_B;
	task1.stack = stack1;

	// Seteo de valores de configuración iniciail para task2
	task2.arg = 0;
	task2.priority = 3;
	task2.entry_point = taskMain2;
	task2.stack_size_bytes = STACK_SIZE_B;
	task2.stack = stack2;

	// Inicialización de tareas
	taskInit(task1, 0);
	taskInit(task2, 1);
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
