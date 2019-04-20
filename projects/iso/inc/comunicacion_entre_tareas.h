#ifndef comunicacion_tareas_h
#define comunicacion_tareas_h

#include "pistaOS.h"





typedef struct {
  char* buffer;
  uint8_t longitud;
}uart_tx_data_t;


extern pistaOS_event_t eventoUartTX_full;
extern uart_tx_data_t mensajeParaTx;


#endif
