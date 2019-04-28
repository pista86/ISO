#ifndef comunicacion_tareas_h
#define comunicacion_tareas_h

#include "pistaOS.h"



typedef struct {
  char* buffer;
  uint8_t longitud;
}uart_tx_data_t;


extern pistaOS_event_t eventoUartTX_full;
extern pistaOS_event_t eventoLedR_ON;
extern pistaOS_event_t eventoLedG_ON;
extern pistaOS_event_t eventoLedB_ON;
extern pistaOS_event_t eventoLedY_ON;

extern uart_tx_data_t mensajeParaTx;

extern uint32_t TonR;
extern uint32_t TonG;
extern uint32_t TonB;
extern uint32_t TonY;


#endif
