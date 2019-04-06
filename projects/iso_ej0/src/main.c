/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "pista_OS.h"

#include <string.h>

/*==================[macros and definitions]=================================*/

#define STACK_SIZE_B 512



/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

uint32_t stack1[STACK_SIZE_B / 4];
uint32_t stack2[STACK_SIZE_B / 4];



/*==================[internal functions definition]==========================*/

static void initHardware(void) {
	boardConfig();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	pista_OS_init();
}



/*==================[external functions definition]==========================*/

void * task1(void * arg) {

	uint32_t i = 0;

	while (1) {
		__WFI();
		if (i == 100) {
			gpioToggle(LED1);
			i = 0;
		}
		i++;
	}

	return NULL;
}

void * task2(void * arg) {

	uint32_t i = 0;

	while (1) {
		__WFI();
		if (i == 500) {
			gpioToggle(LED2);
			i = 0;
		}
		i++;
	}

	return NULL;
}

void task_return_hook(void * ret_val) {
	while (1) {
		__WFI();
	}
}


int main(void) {

	init_stack(stack1, STACK_SIZE_B, &sp1, task1, 0);
	init_stack(stack2, STACK_SIZE_B, &sp2, task2, 0);

	initHardware();

	Board_LED_Toggle(LEDS_LED4);

	while (1) {
		__WFI();
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
