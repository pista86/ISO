
#ifndef _PISTAOS_EVENT_H_
#define _PISTAOS_EVENT_H_

/*==================[inclusions]=============================================*/

#include "pistaOS.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct {
	uint32_t id;
} pistaOS_event_t;



pistaOS_event_t event_init(void);
void event_wait(pistaOS_event_t event_id);
void event_set(pistaOS_event_t event_id);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif
/*==================[end of file]============================================*/
#endif /* #ifndef _PISTAOS_EVENT_H_ */






