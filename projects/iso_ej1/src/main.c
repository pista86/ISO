/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"


#include <string.h>

/*==================[macros and definitions]=================================*/

#define STACK_SIZE_B 512

typedef void *(*task_type)(void *);

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

uint32_t sp1, sp2;

uint32_t current_task;

/*==================[internal functions definition]==========================*/

static void initHardware(void) {
	boardConfig();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

/*==================[external functions definition]==========================*/

void * task1(void * arg) {

	uint32_t i = 0;

	while (1) {
		__WFI();
		if (i == 100) {
			gpioToggle( LED1 );
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
			gpioToggle( LED2 );
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

uint32_t get_next_context(uint32_t current_sp) {
	uint32_t next_sp;

	switch (current_task) {
	case 0:
		// ver que hacer con el contexto inicial
		next_sp = sp1;
		current_task = 1;
		break;
	case 1:
		sp1 = current_sp;
		next_sp = sp2;
		current_task = 2;
		break;
	case 2:
		sp2 = current_sp;
		next_sp = sp1;
		current_task = 1;
		break;
	default:
		while (1) {
			__WFI();
		}
	}

	return next_sp;
}

void init_stack(uint32_t stack[], uint32_t stack_size_bytes, uint32_t * sp,
		task_type entry_point, void * arg) {
	bzero(stack, stack_size_bytes);

	stack[stack_size_bytes / 4 - 1] = 1 << 24; // xPSR.T = 1
	stack[stack_size_bytes / 4 - 2] = (uint32_t) entry_point; // PC
	stack[stack_size_bytes / 4 - 3] = (uint32_t) task_return_hook; // LR
	stack[stack_size_bytes / 4 - 8] = (uint32_t) arg; // R0
	stack[stack_size_bytes / 4 - 9] = 0xFFFFFFF9;

	*sp = (uint32_t) &(stack[stack_size_bytes / 4 - 17]); // considero los otros 8 registros
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
