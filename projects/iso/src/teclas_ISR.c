// sAPI header
#include "sapi.h"
#include "pistaOS.h"
#include "teclas_ISR.h"

static void tec_irq_enable(uint8_t irqChannel, uint8_t port, uint8_t pin,
		irq_edges_t edge);
static void clearInterrupt(uint8_t irqChannel);
static void tec_irq_process(uint8_t irqChannel, uint8_t tec,
		uint32_t tickCount);
static tecla_event_callback_t tecla_event_callback;

void teclas_irq_enable(tecla_event_callback_t teclaEventCallback) {

	tecla_event_callback = teclaEventCallback;

	tec_irq_enable(4, 0, 4, BOTH_EDGE); // TEC1
	tec_irq_enable(5, 0, 8, BOTH_EDGE); // TEC2
	tec_irq_enable(6, 0, 9, BOTH_EDGE); // TEC3
	tec_irq_enable(7, 1, 9, BOTH_EDGE); // TEC4
}

static void tec_irq_enable(uint8_t irqChannel, uint8_t port, uint8_t pin,
		irq_edges_t edge) {

	Chip_SCU_GPIOIntPinSel(irqChannel, port, pin);

	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
	/* Set edge interrupt mode */
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(irqChannel));

	if (edge == RAISING_EDGE) {
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
	} else if (edge == FALLING_EDGE) {

		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
	} else {
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
	}

	/* Clear pending irq channel interrupts */
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn + irqChannel);

	NVIC_SetPriority(PIN_INT0_IRQn + irqChannel, 7); //Debe ser mayor (número) a configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

	/* Enable irqChannel interrupt */
	NVIC_EnableIRQ(PIN_INT0_IRQn + irqChannel);
}

static void clearInterrupt(uint8_t irqChannel) {
	/* Clear interrupt flag for irqChannel */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
}

static void tec_irq_process(uint8_t irqChannel, uint8_t tec,
		uint32_t tickCount) {
	tecla_event_t eventoTecla;

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH(irqChannel)) {
		// flanco ascendente
		eventoTecla = TECLA_UP;
		Chip_PININT_ClearRiseStates(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
	} else {
		// flanco descendente
		eventoTecla = TECLA_DOWN;
		Chip_PININT_ClearFallStates(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
	}

	tecla_event_callback(tec, eventoTecla);

	/* Clear IRQ status */
	clearInterrupt(irqChannel);
}

void GPIO4_IRQHandler(void) {
	tec_irq_process(4, 1, get_sysTickCount());
}

void GPIO5_IRQHandler(void) {
	tec_irq_process(5, 2, get_sysTickCount());
}

void GPIO6_IRQHandler(void)
{
	tec_irq_process(6, 3, get_sysTickCount());
}

void GPIO7_IRQHandler(void)
{
	tec_irq_process(7, 4, get_sysTickCount());
}
