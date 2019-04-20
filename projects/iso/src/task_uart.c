#include "task_uart.h"

#include "sapi.h"

static void uartRxCallback(void);
void uartTxCallback(void);
static void uartTxSendByte(uint8_t byte);
static void uartSend(uint8_t* buffer);

static uint32_t txIndex;
uart_tx_data_t mensajeParaTx;


pistaOS_event_t eventoUartTX_empty;
pistaOS_event_t eventoUartTX_full;

// Implementacion de funcion de la tarea
void * uart_task(void* arg) {

	eventoUartTX_empty = event_init();
	eventoUartTX_full = event_init();


	uartInit( UART_USADA, UART_USADA_SPEED);

	uartRxInterruptCallbackSet( UART_USADA, (callBackFuncPtr_t) uartRxCallback);
	uartTxInterruptCallbackSet( UART_USADA, (callBackFuncPtr_t) uartTxCallback);

	uartRxInterruptSet(UART_USB, true);
	uartTxInterruptSet(UART_USB, true);

	while (TRUE) {
		event_wait(eventoUartTX_full);

		uartSend(mensajeParaTx.buffer);
		// no vuelvo a iniciar una transmisión hasta tanto se termine de transmitir el buffer entero

		event_wait(eventoUartTX_empty);
	}
}


static void uartSend(uint8_t* buffer) {
	txIndex = 0;

	uartTxSendByte(buffer[txIndex++]);
}

static uint8_t uartGetTxData(uint8_t* txData) {

	uint8_t result = 0;

	if (txIndex < mensajeParaTx.longitud) {
		*txData = mensajeParaTx.buffer[txIndex];
		txIndex++;
		result = 1;
	} else {
		// paquete transmitido completo

		event_set(eventoUartTX_empty);
	}

	return result;
}

static void uartTxSendByte(uint8_t byte) {
	uartTxWrite(UART_USADA, byte);

}

static void uartRxCallback(void) {




}

void uartTxCallback(void) {
	uint8_t txData;

	if (uartGetTxData(&txData)) {
		uartTxSendByte(txData);
	}
}
