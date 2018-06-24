#include "common.h"

#ifdef HAS_IOE

#include "device/mmio.h"

#define VMEM 0x40000

#define SCREEN_H 300
#define SCREEN_W 400

static uint32_t (*vmem) [SCREEN_W];


void vga_io_handler(paddr_t addr, int len, bool is_write) {
	assert(is_write);
	int w = len / 4;
	int h = 1;
	int y = (addr - VMEM) / (SCREEN_W * 4);
	int x = (addr - VMEM) % (SCREEN_W * 4) / 4;
	uint32_t *pixels = &vmem[y][x];
	draw_rect(pixels, x, y, w, h);

	static int counter = 0;
	counter ++;
	if(counter > 10000) { // delayed sync
		counter = 0;
		draw_sync();
	}
}

void init_vga() {
	vmem = add_mmio_map(VMEM, 0x80000, vga_io_handler);
}

#endif	/* HAS_IOE */
