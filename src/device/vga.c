#include "common.h"

#ifdef HAS_IOE

#include "device/mmio.h"
// #include <SDL2/SDL.h>

#define VMEM 0x40000

#define SCREEN_H 300
#define SCREEN_W 400

// static SDL_Window *window;
// static SDL_Renderer *renderer;
// static SDL_Texture *texture;
 
static uint32_t (*vmem) [SCREEN_W];
 
// void update_screen() {
//   SDL_UpdateTexture(texture, NULL, vmem, SCREEN_W * sizeof(vmem[0][0]));
//   SDL_RenderClear(renderer);
//   SDL_RenderCopy(renderer, texture, NULL, NULL);
//   SDL_RenderPresent(renderer);
// }
 

void vga_io_handler(paddr_t addr, int len, bool is_write)
{
	assert(is_write);
	int w = len / 4;
	int h = 1;
	int y = (addr - VMEM) / (SCREEN_W * 4);
	int x = (addr - VMEM) % (SCREEN_W * 4) / 4;
	uint32_t *pixels = &vmem[y][x];
	draw_rect(pixels, x, y, w, h);

//	draw_sync();

}

void init_vga() {
//   SDL_Init(SDL_INIT_VIDEO);
//   SDL_CreateWindowAndRenderer(SCREEN_W * 2, SCREEN_H * 2, 0, &window, &renderer);
//   SDL_SetWindowTitle(window, "NEMU");
//   texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
//       SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
 
     vmem = add_mmio_map(VMEM, 0x80000, vga_io_handler);
}
#endif	/* HAS_IOE */
