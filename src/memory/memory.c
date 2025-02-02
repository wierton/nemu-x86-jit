#include "nemu.h"
#include "device/mmio.h"

#define PMEM_SIZE (2 * 1024 * 1024)

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  int no = is_mmio(addr);
  if (no == -1) {
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr);
	uint32_t data = 0;
	memcpy(&data, guest_to_host(addr), 4);
	return data & (~0u >> ((4 - len) << 3));
  } else {
	return mmio_read(addr, len, no);
  }
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
  int no = is_mmio(addr);
  if (no == -1)
	memcpy(guest_to_host(addr), &data, len);
  else
	mmio_write(addr, len, data, no);
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
  paddr_write(addr, len, data);
}
