#include "medicion.h"



static measure_tec_t measure_tec;

static void send_diff(measure_tec_t medicionTec);
static uint32_t safe_diff(uint32_t A, uint32_t B);
static void send_to_edge_led(uint8_t index);

void medicionInit(void)
{

	measure_tec.measute_edge = MEASURE_NONE;
	measure_tec.firstTec = -1;


}

void tec_up(tec_t tec, uint8_t index)
{

	if (measure_tec.firstTec != index)
	{

		switch (measure_tec.measute_edge)
		{

		case MEASURE_NONE:
			// comienzo medicion
			measure_tec.measute_edge = MEASURE_RISING;
			measure_tec.firstTecTick = tec.tickUp;
			measure_tec.firstTec = index;
			break;

		case MEASURE_RISING:
			// fin de medición tiempo entre flancos descendentes
			measure_tec.secondTecTick = tec.tickUp;
			measure_tec.secondTec = index;
			send_diff(measure_tec);
			measure_tec.measute_edge = MEASURE_NONE;
			measure_tec.firstTec = -1;
			break;

		}

	}
	else
	{
		measure_tec.measute_edge = MEASURE_NONE;
		measure_tec.firstTec = -1;
	}

	send_to_edge_led(index);

}

void tec_down(tec_t tec, uint8_t index)
{
	if (measure_tec.firstTec != index)
	{
		switch (measure_tec.measute_edge)
		{

		case MEASURE_NONE:
			// comienzo medicion
			measure_tec.measute_edge = MEASURE_FALLING;
			measure_tec.firstTecTick = tec.tickDown;
			measure_tec.firstTec = index;
			break;

		case MEASURE_FALLING:
			// fin de medición tiempo entre flancos descendentes
			measure_tec.secondTecTick = tec.tickDown;
			measure_tec.secondTec = index;
			send_diff(measure_tec);
			measure_tec.measute_edge = MEASURE_NONE;
			measure_tec.firstTec = -1;
			break;

		}
	}
	else
	{
		measure_tec.measute_edge = MEASURE_NONE;
		measure_tec.firstTec = -1;
	}

	send_to_edge_led(index);

}

static void send_to_edge_led(uint8_t index)
{

	switch (index)
	{

	case 0:

		break;

	case 1:

		break;
	}

}

static void send_diff(measure_tec_t medicionTec)
{

//	sprintf(printFln_queue_message.string, "{%d:%d:%d}", medicionTec.firstTec, medicionTec.secondTec,
//			safe_diff(medicionTec.secondTecTick, medicionTec.firstTecTick) / portTICK_RATE_MS);


}

static uint32_t safe_diff(uint32_t A, uint32_t B)
{
	// A-B de ticks sin errores
	if (A < B)
	{
		return (UINT32_MAX - B + A );
	}
	else
	{
		return A - B;
	}

}

