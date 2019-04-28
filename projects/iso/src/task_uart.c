#include "task_uart.h"

void uartTxCallback(void);
static void uartTxSendByte(uint8_t byte);
static void uartSend(char* buffer);

static uint32_t txIndex;
static uint8_t sending;
static char txMessage[MAX_TX_MESSAGE_LENGTH];
static uint8_t txMessageLength;
uart_tx_data_t mensajeParaTx;

pistaOS_event_t eventoUartTX_empty;
pistaOS_event_t eventoUartTX_full;

// Implementacion de funcion de la tarea
void * uart_task(void* arg) {

	sending = 0;

	eventoUartTX_empty = event_init();
	eventoUartTX_full = event_init();

	uartInit( UART_USADA, UART_USADA_SPEED);

	uartTxInterruptCallbackSet( UART_USADA, (callBackFuncPtr_t) uartTxCallback);

	uartTxInterruptSet(UART_USB, true);

	while (TRUE) {
		event_wait(&eventoUartTX_full);

		// copio mensaje a enviar a buffer de salida
		memcpy(txMessage, mensajeParaTx.buffer, mensajeParaTx.longitud);
		txMessageLength = mensajeParaTx.longitud;

		uartSend(txMessage);
		// no vuelvo a iniciar una transmisión hasta tanto se termine de transmitir el buffer entero

		event_wait(&eventoUartTX_empty);
	}
}

static void uartSend(char* buffer) {
	txIndex = 0;
	sending = 1;

	uartTxSendByte(buffer[txIndex++]);
}

static uint8_t uartGetTxData(uint8_t* txData) {

	uint8_t result = 0;

	if (txIndex < txMessageLength) {
		*txData = txMessage[txIndex];
		txIndex++;
		result = 1;
	} else {
		// paquete transmitido completo

		event_set(&eventoUartTX_empty);
	}

	return result;
}

static void uartTxSendByte(uint8_t byte) {
	uartTxWrite(UART_USADA, byte);
}

void uartTxCallback(void) {
	uint8_t txData;
	if (sending) {
		if (uartGetTxData(&txData)) {
			uartTxSendByte(txData);
		} else {
			sending = 0;
		}
	}
}
