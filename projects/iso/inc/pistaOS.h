#ifndef _PISTAOS_H_
#define _PISTAOS_H_

/*==================[inclusions]=============================================*/

#include "cmsis.h"
#include "core_cm4.h"
#include <string.h>

#include "pistaOS_config.h"
#include "pistaOS_task.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C"
{
#endif


void OSInit(void);

void taskInit(pistaOS_task_t task, uint8_t taskNumber);

void taskDelay(uint32_t delayTicks);


/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif
/*==================[end of file]============================================*/
#endif /* #ifndef _PISTAOS_H_ */
