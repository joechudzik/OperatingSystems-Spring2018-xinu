/* TA-BOT:MAILTO joseph.chudzik@marquette.edu */
// Joe Chudzik and Adam Shakal
/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:

//  Save callee-save ("non-volatile") registers.
//  Restore callee-save ("non-volatile") registers.
//  Jump to next function.
 
// START

	mrs r12, cpsr
	push {r0-r12}
	
	str sp, [r0]
	ldr sp, [r1]
	
	pop {r0, r12}
	msr cpsr_c, r12
	pop {lr, pc}

//  END

	bx	lr

	.end ctxsw
