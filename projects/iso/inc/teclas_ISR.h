
#pragma once



typedef enum {
	RAISING_EDGE,
	FALLING_EDGE,
	BOTH_EDGE,
} irq_edges_t;

typedef enum {
	TECLA_UP, TECLA_DOWN
} tecla_event_t;

typedef void (*tecla_event_callback_t)(uint8_t tecla, tecla_event_t evento);

void GPIO4_IRQHandler(void);
void GPIO5_IRQHandler(void);
//void GPIO6_IRQHandler(void);
//void GPIO7_IRQHandler(void);

void teclas_irq_enable(tecla_event_callback_t teclaEventCallback);
