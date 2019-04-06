#include "pista_OS.h"

#include "cmsis_43xx.h"
#include "core_cm4.h"

static void schedule(void);


uint32_t sp1, sp2;

uint32_t current_task;


void pista_OS_init(void) {

	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

}

void SysTick_Handler(void) {

	schedule();

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



static void schedule(void) {

	__ISB();

	__DSB();

	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

}
