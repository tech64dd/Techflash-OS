#include <kernel/COM.h>
#include <kernel/SMP.h>
#include <kernel/tty.h>
#include <kernel/mem.h>
#include <kernel/arch.h>
#include <kernel/bootloader.h>
#include <kernel/tty/fbCon.h>
#include <stdio.h>
#include <string.h>


void __attribute__((noreturn)) main() {
	TTY_SetWriteFunc(COM_LogWrapper);
	BOOT_CheckLoader();
	SMP_Init();
	ARCH_Init();
	PMM_Init();

	FBCON_Init();


	puts("Testing PMM!\r\n");

	void *ptr = PMM_Alloc(5);
	puts("1234567890");
	printf("Recieved pointer: %p\r\n", ptr);
	memset(ptr, 0xA5, 5 * 4096);
	puts("Kernel execution done.  Hanging.\r\n");
	while (true) {asm ("cli; hlt");}
}