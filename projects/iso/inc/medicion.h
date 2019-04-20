
#pragma once

#include "sapi.h"
#include "task_debounce.h"


typedef enum {
	MEASURE_NONE,
	MEASURE_RISING,
	MEASURE_FALLING,
} measure_edge_t;

typedef struct{
	measure_edge_t	measute_edge;
	int8_t firstTec;
	int8_t secondTec;
	uint32_t firstTecTick;
	uint32_t secondTecTick;
} measure_tec_t;


void medicionInit(void);
void tec_up(tec_t tec, uint8_t index);
void tec_down(tec_t tec, uint8_t index);
