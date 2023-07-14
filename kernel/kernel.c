/*
	Original version Copyright 2017 - 2021 bzt (bztsrc@gitlab)
	as part of the 'bootboot' repository on GitLab.
	Original file available at https://gitlab.com/bztsrc/bootboot/-/blob/master/mykernel/c/kernel.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <kernel/environment.h>
#include <kernel/tty.h>
#include <kernel/hardware/serial.h>
#include <kernel/hardware/parallel.h>
#include <kernel/hardware/PIT.h>
#include <kernel/boot.h>
#include <kernel/graphics.h>
#include <kernel/hardware/CPU/x86Setup.h>
#include <kernel/panic.h>
#include <kernel/hardware/kbd.h>
#include <kernel/fs/tar.h>
#include <kernel/shell.h>
#include <kernel/memory.h>
#include <kernel/elf.h>
#include <kernel/syscall.h>

#include <kernel/custom.h>
MODULE("KERNEL");
kernel_t kernel;

uint8_t SSEFeaturesBits = 0;
extern void initThings();
extern void initExceptions();
extern void PICInit();
extern bool timerReady;
static uint8_t whatCoreAmI() {
	uint64_t rbx = 0;
	asm (
		"mov $1, %%rax;"
		"cpuid;"
		"movq %%rbx, %0;"
		: "=r" (rbx)
		:
	);
	return (rbx >> 24);
}
uint8_t numCPUs = 1;
extern void testMalloc();
// cppcheck-suppress unusedFunction
void kernelMain() {
	// before we do anything, if we're not the bootstrap processor, just hang.
	// in future versions, they'll do things, for now, they won't
	if (whatCoreAmI() != bootboot.bspid) {
		// non bootstrap processor
		numCPUs++;
		asm ("cli;hlt");
	}
	SSEInit();


	initThings();
	// Initialize the physical memory manager
	PMM_Init();
	testMalloc();

	KBD_SetLED(KEYBOARD_LED_NUMLOCK, true);

	// Say that the kernel is loading and to please wait.
	// if (env.experimental.progressBarBoot) {
	uint_fast8_t bootX = ((TTY_Width / 2) - (TTY_Width / 3)); // idk it looks centered to me
	uint_fast8_t bootY = ((TTY_Height / 2) + (TTY_Height / 4)); // don't forget, the Y goes up the farther down the screen you are.  this means 3/4 down the screen (1/2 + 1/4)
	BP_Init(bootX, bootY, TTY_Width / 2);

	// Initialize the 8259 Programmable Interrupt Controller
	PICInit();
	BP_Update();

	// Initialize the Global Descriptor Table
	GDTInit();
	BP_Update();

	// Initialize the Interrupt Descriptor Table
	IDTInit();
	BP_Update();


	// TODO: Distribute these               vvv !
	// Initialize some exception handlers
	initExceptions();
	BP_Update();

	// Init the keyboard driver
	KBD_Init();
	BP_Update();

	// Initialize the PIT to once every 1ms
	initPIT(1000);
	BP_Update();

	
	KBD_SetIntState(0, true);
	
	// Now the interrupts are ready, enable them
	log(MODNAME, "INTERRUPTS ARE BEING ENABLED!!!", LOGLEVEL_WARN);
	asm volatile ("sti");
	timerReady = true;
	BP_Update();

	// Initialize the parallel port (we need interrupts for this, since it has a timeout)
	parallel.init();
	BP_Update();

	initSyscalls();
	BP_Update();
	parseTar((void *)bootboot.initrd_ptr);
	BP_Update();
	handleEnv();

	printf("Found \e[1;36m%u\e[0m CPUs!\r\n", numCPUs);
	
	sleep(250);
	
	BP_FadeOut();
	TTY_BlinkingCursor = true;

	while (true) {
		// TODO: Literally anything
	}
	asm volatile (
		"cli\n"
		"hlt\n"
	);
}
