	.syntax unified
	.text
	.global PendSV_Handler
	.extern get_next_context
	.thumb_func

PendSV_Handler:

	/* contexto FPU */
	tst lr,0x10 /* lr and 0x10 comparando */
	it eq /* si el resultado es igual cero*/
	vpusheq {s16-s31} /* push delos registros que faltan*/
	/* contexto FPU */

	push {r4-r11, lr}

	mrs r0, msp	// r0 = msp
	bl get_next_context // get_next_context(msp)
	msr	msp, r0	// msp = ro, valor devuelto por get_next_context(msp)

	pop {r4-r11, lr}

	/* contexto FPU */
	tst lr,0x10
	it eq
	vpopeq {s16-s31}
	/* contexto FPU */

return:
	bx	lr
