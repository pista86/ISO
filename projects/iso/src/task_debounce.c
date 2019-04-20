#include "task_debounce.h"


static tec_t teclas[CANT_TECLAS];

static void task_init(void);
static void procesamiento_teclas(uint8_t tecla, tecla_event_t evento);
static void process_debounce(void);
static uint8_t bouncing(void);
static void tec_status_init(void);
static bool_t read_tecla(uint8_t index);

static void task_init(void) {

	teclas_irq_enable(procesamiento_teclas);

	tec_status_init();
}

void * task_debounce(void * arg) {



	task_init();

	while (1) {
		__WFI();

		process_debounce();

		taskDelay(10);

	}

	return NULL;
}

static void process_debounce(void) {
	uint8_t i;

	for (i = 0; i < CANT_TECLAS; i++) {
		if (teclas[i].status == TEC_FALLING) {
			// si ya pasó el tiempo de debounce leo la tecla
			if (((get_sysTickCount() - teclas[i].tickCount) / TICK_RATE_MS)
					> DEBOUNCE_PERIOD_MS) {
				if (read_tecla(i) == FALSE) {
					// sigue abajo no fue rebote, está down, tomo el tiempo actual como referencia
					teclas[i].tickDown = get_sysTickCount();
					teclas[i].status = TEC_DOWN;
					//tec_down(teclas[i], i);
					gpioToggle(LED1);
				} else {
					// subió, fue rebote, está up
					teclas[i].status = TEC_UP;
				}
			}

		} else if (teclas[i].status == TEC_RAISING) {
			// si ya pasó el tiempo de debounce leo la tecla
			if (((get_sysTickCount() - teclas[i].tickCount) / TICK_RATE_MS)
					> DEBOUNCE_PERIOD_MS) {
				if (read_tecla(i) == TRUE) {
					// sigue arriba no fue rebote, está up, tomo el tiempo actual como referencia
					teclas[i].tickUp = get_sysTickCount();
					teclas[i].status = TEC_UP;
					//tec_up(teclas[i], i);
					gpioToggle(LED2);
				} else {
					// bajó, fue rebote, está down
					teclas[i].status = TEC_DOWN;
				}
			}

		}

	}

}

static uint8_t bouncing(void) {
	uint8_t i;
	uint8_t result = 0;

	for (i = 0; i < CANT_TECLAS; i++) {
		if (teclas[i].status == TEC_FALLING
				|| teclas[i].status == TEC_RAISING) {
			result = 1;
			break;
		}
	}

	return result;
}

static void tec_status_init(void) {
	uint8_t i;

	for (i = 0; i < CANT_TECLAS; i++) {
		if (read_tecla(i)) {
			teclas[i].status = TEC_UP;
		} else {
			teclas[i].status = TEC_DOWN;
		}
	}
}

static bool_t read_tecla(uint8_t index) {
	return gpioRead(TEC1 + index);
}

void procesamiento_teclas(uint8_t tecla, tecla_event_t evento) {



	switch (evento) {

	case TECLA_UP:

		teclas[tecla - 1].status = TEC_RAISING;

		break;
	case TECLA_DOWN:

		teclas[tecla - 1].status = TEC_FALLING;

		break;

	}

}
