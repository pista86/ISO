
#pragma once

#include "sapi.h"
#include "pistaOS.h"
#include "comunicacion_entre_tareas.h"


#define UART_USADA				UART_USB
#define UART_USADA_SPEED		115200

#define MAX_TX_MESSAGE_LENGTH	256

typedef void (*tarea_uart_tx_complete_callback_t)(void);

void * uart_task(void* taskParmPtr);




