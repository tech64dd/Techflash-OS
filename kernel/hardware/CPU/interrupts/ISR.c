#include <kernel/hardware/CPU/regs.h>
#include <kernel/hardware/CPU/ISR.h>
#include <kernel/hardware/IO.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <kernel/tty.h>
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20
isr_t interruptHandlers[256];
uint64_t lastInterruptNumber = 0;
void sendEOI() {
	if(lastInterruptNumber >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
 
	outb(PIC1_COMMAND, PIC_EOI);
}
void ISRHandlersInit() {
	for (int i = 0; i < 256; i++) {
		interruptHandlers[i] = 0;
	}
}
void ISRHandler(registers_t* regs) {
	regs->intNo += 128;
	lastInterruptNumber = regs->intNo;
	if (interruptHandlers[regs->intNo] != 0) {
		isr_t handler = interruptHandlers[regs->intNo];
		handler(regs);
	}
	else {
		panic("Unhandled Interrupt", *regs);
	}
}
void registerInterruptHandler(uint8_t n, isr_t handler) {
	interruptHandlers[n] = handler;
}

