
#pragma once

#include "sapi.h"
#include "teclas_ISR.h"
#include "pistaOS.h"


#define CANT_TECLAS	2

#define DEBOUNCE_PERIOD_MS	50


typedef enum {
	TEC_RAISING,
	TEC_UP,
	TEC_FALLING,
	TEC_DOWN,
} tec_status_t;

typedef struct{
	uint32_t tickCount;
	tec_status_t status;
} tec_t;






void * task_debounce(void * arg);


