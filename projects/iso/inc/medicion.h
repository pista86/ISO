
#pragma once

#include "sapi.h"
#include "task_debounce.h"


typedef enum {
	MEASURE_WAITING_1,
	MEASURE_WAITING_2,
	MEASURE_WAITING_3,
	MEASURE_WAITING_4
} measure_status_t;

typedef struct{
	int8_t eventTec_1;
	int8_t eventTec_2;
	int8_t eventTec_3;
	int8_t eventTec_4;
	uint32_t eventTick_1;
	uint32_t eventTick_2;
	uint32_t eventTick_3;
	uint32_t eventTick_4;
} measure_data_t;


void medicionInit(void);
void tec_up(tec_t tec, uint8_t index);
void tec_down(tec_t tec, uint8_t index);
