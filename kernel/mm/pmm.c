#include <external/limine.h>
#include <kernel/mem.h>
#include <kernel/bootloader.h>
#include <kernel/registers.h>
#include <kernel/panic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
MODULE("PMM");
extern memmap_t *LM_ParseMemmap();
static memmap_t *BOOT_ParseMemmap() {
	if (BOOT_LoaderID == BOOT_LoaderID_LimineCompatible) {
		return LM_ParseMemmap();
	}
	registers_t regs;
	DUMPREGS(&regs);
	log(MODNAME, "Unkown Bootloader!  No idea where the memory map is.  Giving up.", LOGLEVEL_FATAL);
	panic("No memory map detected", &regs);
	return NULL;
}

static void PMM_CalcSizeStr(char *sizeStr, size_t size) {
	uint_fast8_t i;
	size_t oldSize, decimal;
	char *types = "BKMGTP???";
	for (i = 0; i != 6; i++) {
		if ((size % 1024) == 0) {
			oldSize = size;
			size /= 1024;
			continue;
		}
		if ((size / 1024) >= 10) {
			oldSize = size;
			size /= 1024;
			continue;
		}
		break;
	}
	decimal = oldSize % 1024;
	if (decimal > 0) {
		sprintf(sizeStr, "%ld.%ld%c", size, (decimal * 100) / 1024, types[i]);
	}
	else {
		sprintf(sizeStr, "%ld%c", size, types[i]);
	}
}

void PMM_Init() {
	log(MODNAME, "PMM Initializing", LOGLEVEL_INFO);
	log(MODNAME, "Getting memory map from bootloader...", LOGLEVEL_DEBUG);
	log(MODNAME, "====== Memory Map ======", LOGLEVEL_DEBUG);
	char str[128];

	memmap_t *memmap = BOOT_ParseMemmap();

	sprintf(str, "Got memory map with %d entries at %p", memmap->numEntries, memmap);
	log(MODNAME, str, LOGLEVEL_DEBUG);

	for (uint_fast8_t i = 0; i != memmap->numEntries; i++) {
		/* TODO: Make size & type strings with the following logic:
		- `(size % 1024) == 0`
			- YES, Cleanly divisible by 1024, just divide by 1024 and repeat the loop
			- NO, Not cleanly divisible by 1024, continue below
		- `(size / 1024) >= 10`
			- YES, dividing by 1024 gives a number greater than 10, just divide by 1024 and repeat the loop, keeping track of the decimal portion of the number.
			- NO, dividing by 1024 gives a number less than 10, continue below
		- Is the number a clean integer, with no remaining decimal places?  (e.g. 10)
			- YES, treat it as a basic integer, don't include the decimal portion in the string.
			- NO, it's a decimal number, include the decimal portion in the string
		- By this point, the current state of the number should be: Greater than 10, less than 1024, either a decimal number, or standard integer.
		- Calculate the size, adding the suffix of `B` (bytes), `K` (KB), `M` (MB), `G`, (GB), `T`, (TB), `P`, (PB). 
		*/
		uint64_t size = memmap->entries[i].size;
		char sizeStr[16];
		char *typeStr = "typeStr";

		// Make size & type strings
		PMM_CalcSizeStr(sizeStr, size);

		char *space = "";
		if ((memmap->numEntries >= 10) && (i < 10)) {
			space = " ";
		}
		sprintf(str, "Entry %d%s: %p - %p; %s; Type: %s", i, space, memmap->entries[i].start, memmap->entries[i].start + memmap->entries[i].size, sizeStr, typeStr);
		log(MODNAME, str, LOGLEVEL_DEBUG);
	}
}