#include <external/limine.h>
#include <stdbool.h>

struct limine_smp_request smpRequest = {
	.id = LIMINE_SMP_REQUEST,
	.revision = 0,
	.flags = 1
};

void main() {
	while (true) {};
}