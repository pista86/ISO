#ifndef _PISTAOS_H_
#define _PISTAOS_H_

/*==================[inclusions]=============================================*/

#include "cmsis.h"
#include "core_cm4.h"
#include <string.h>

#include "pistaOS_config.h"
#include "pistaOS_task.h"
#include "pistaOS_event.h"

#define TICK_RATE_MS 1000

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C"
{
#endif



void OSInit(void);

void taskInit(pistaOS_task_t task, uint8_t taskNumber);

uint32_t get_running_task_index(void);
void set_task_status(uint32_t taskIndex, pistaOS_task_status_t status);
uint32_t get_sysTickCount(void);

void taskDelay(uint32_t delayTicks);


/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif
/*==================[end of file]============================================*/
#endif /* #ifndef _PISTAOS_H_ */
