#include "medicion.h"
#include "task_uart.h"

static measure_data_t measure_data;
static measure_status_t measure_status;

static void mef_process(tec_t tec, uint8_t index);
static uint32_t safe_diff(uint32_t A, uint32_t B);
static void send_result(measure_data_t measure);

static char txcadena[256];

void medicionInit(void) {

	// Estado inicial de mef
	measure_status = MEASURE_WAITING_1;

}

void tec_up(tec_t tec, uint8_t index) {

	// Procesar tecla presionada
	mef_process(tec, index);

}

void tec_down(tec_t tec, uint8_t index) {

	// Procesar tecla levantada
	mef_process(tec, index);

}

static void mef_process(tec_t tec, uint8_t index) {

	switch (measure_status) {

	case MEASURE_WAITING_1:
		// comienzo medicion

		if (tec.status == TEC_DOWN) {

			measure_data.eventTec_1 = index;
			measure_data.eventTick_1 = get_sysTickCount();

			measure_status = MEASURE_WAITING_2;
		}

		break;

	case MEASURE_WAITING_2:

		if (index != measure_data.eventTec_1 && tec.status == TEC_DOWN) {
			// Si el evento proviene de otra tecla proceso
			measure_data.eventTec_2 = index;
			measure_data.eventTick_2 = get_sysTickCount();

			measure_status = MEASURE_WAITING_3;
		} else {
			// Segundo evento proviene de la misma tecla, error de secuencia
			measure_status = MEASURE_WAITING_1;
		}

		break;
	case MEASURE_WAITING_3:

		measure_data.eventTec_3 = index;
		measure_data.eventTick_3 = get_sysTickCount();

		measure_status = MEASURE_WAITING_4;
		break;
	case MEASURE_WAITING_4:

		measure_data.eventTec_4 = index;
		measure_data.eventTick_4 = get_sysTickCount();

		send_result(measure_data);

		measure_status = MEASURE_WAITING_1;
		break;

	}

}

static void send_result(measure_data_t measure) {
	uint32_t t1, t2;
	mensajeParaTx.buffer = txcadena;

	t1 = safe_diff(measure.eventTick_2, measure.eventTick_1);
	t2 = safe_diff(measure.eventTick_4, measure.eventTick_3);

	if (measure.eventTec_1 == 0 && measure.eventTec_2 == 1
			&& measure.eventTec_3 == 0 && measure.eventTec_4 == 1) {
		// Led verde
		TonG = t1 + t2;
		// Activo led
		event_set(&eventoLedG_ON);

		mensajeParaTx.longitud =
				sprintf(txcadena,
						"Led Verde encendido:\n\r \t Tiempo encendido: %lu ms \n\r \t Tiempo entre flancos descendentes: %lu ms \n\r \t Tiempo entre flancos ascendentes: %lu ms \n\r",
						t1 + t2, t1, t2);

		// Activo envio por uart
		event_set(&eventoUartTX_full);

	} else if (measure.eventTec_1 == 0 && measure.eventTec_2 == 1
			&& measure.eventTec_3 == 1 && measure.eventTec_4 == 0) {
		//Led rojo
		TonR = t1 + t2;
		// Activo led
		event_set(&eventoLedR_ON);

		mensajeParaTx.longitud =
				sprintf(txcadena,
						"Led Rojo encendido:\n\r \t Tiempo encendido: %lu ms \n\r \t Tiempo entre flancos descendentes: %lu ms \n\r \t Tiempo entre flancos ascendentes: %lu ms \n\r",
						t1 + t2, t1, t2);

		// Activo envio por uart
		event_set(&eventoUartTX_full);

	} else if (measure.eventTec_1 == 1 && measure.eventTec_2 == 0
			&& measure.eventTec_3 == 0 && measure.eventTec_4 == 1) {
		//Led amarillo
		TonY = t1 + t2;
		// Activo led
		event_set(&eventoLedY_ON);

		mensajeParaTx.longitud =
				sprintf(txcadena,
						"Led Amarillo encendido:\n\r \t Tiempo encendido: %lu ms \n\r \t Tiempo entre flancos descendentes: %lu ms \n\r \t Tiempo entre flancos ascendentes: %lu ms \n\r",
						t1 + t2, t1, t2);

		// Activo envio por uart
		event_set(&eventoUartTX_full);

	} else if (measure.eventTec_1 == 1 && measure.eventTec_2 == 0
			&& measure.eventTec_3 == 1 && measure.eventTec_4 == 0) {
		// Led azul
		TonB = t1 + t2;
		// Activo led
		event_set(&eventoLedB_ON);

		mensajeParaTx.longitud =
				sprintf(txcadena,
						"Led Azul encendido:\n\r \t Tiempo encendido: %lu ms \n\r \t Tiempo entre flancos descendentes: %lu ms \n\r \t Tiempo entre flancos ascendentes: %lu ms \n\r",
						t1 + t2, t1, t2);

		// Activo envio por uart
		event_set(&eventoUartTX_full);

	}

}

static uint32_t safe_diff(uint32_t A, uint32_t B) {
	// A-B de ticks sin errores
	if (A < B) {
		return (UINT32_MAX - B + A);
	} else {
		return A - B;
	}

}

