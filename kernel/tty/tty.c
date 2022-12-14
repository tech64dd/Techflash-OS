#include <kernel/tty.h>
#include <kernel/font.h>
#include <kernel/environment.h>
#include <external/bootboot.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/graphics.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
void kernTTY_init() {
	kernTTY.ready = false;
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	kernTTY.color = vga.colors.lgray; // VGA light gray.
	kernTTY.width = ((bootboot.fb_width / font->width) * 0.885);
	kernTTY.height = ((bootboot.fb_height / font->height) * 0.99);
	kernTTY.promptStr = malloc(256);
	strcpy(kernTTY.promptStr, "> ");
}
void kernTTY_setBackground(const uint32_t color) {
	kernTTY.textBackground = color;
	int s = bootboot.fb_scanline;
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t*)(&fb + s * y + x * 4)) = color;
		}
	}
}
void kernTTY_clear() {
	int s = bootboot.fb_scanline;
	// Set cursor to top left corner.
	kernTTY.cursorX = 0;
	kernTTY.cursorY = 0;
	// Fill the screen with the background color.
	for (uint32_t y = 0; y < bootboot.fb_height; y++) {
		for (uint32_t x = 0; x < bootboot.fb_width; x++) {
			*((uint32_t*)(&fb + s * y + x * 4)) = kernTTY.textBackground;
		}
	}
}
#pragma GCC diagnostic pop

void kernTTY_printPrompt() {
	if (kernTTY.cursorAfterPromptX != 0) {
		puts("\r\n");
	}
	puts(kernTTY.promptStr);
	kernTTY.cursorAfterPromptX = 0;
}