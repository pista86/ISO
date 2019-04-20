
#ifndef _PISTAOS_TASK_H_
#define _PISTAOS_TASK_H_

/*==================[inclusions]=============================================*/

#include "cmsis.h"
#include "pistaOS_config.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*task_type)(void *);
typedef void *(*return_task_type)(void *);

typedef enum {
	TASK_READY, TASK_RUNNING, TASK_WAITING, TASK_SUSPENDED
} pistaOS_task_status_t;

typedef struct {
	task_type entry_point;
	return_task_type return_point;
	uint32_t * stack;
	uint32_t stack_size_bytes;
	void * arg;
	pistaOS_task_status_t status;
	uint32_t sp;
	uint32_t priority;
	uint32_t delayTicks;
} pistaOS_task_t;

void * idleTask(void * arg);
void initTasks(void);

pistaOS_task_t taskList[PISTAOS_MAX_TASK];

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif
/*==================[end of file]============================================*/
#endif /* #ifndef _PISTAOS_TASK_H_ */

