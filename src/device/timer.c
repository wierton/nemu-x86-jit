#include "device/port-io.h"
#include "monitor/monitor.h"
// #include <sys/time.h>
#include <klib.h>

#define RTC_PORT 0x48   // Note that this is not the standard

void timer_intr() {
  if (nemu_state == NEMU_RUNNING) {
    extern void dev_raise_intr(void);
    dev_raise_intr();
  }
}

static uint32_t *rtc_port_base;

void rtc_io_handler(ioaddr_t addr, int len, bool is_write) {
  if (!is_write) {
    rtc_port_base[0] = uptime();

  }
}

void init_timer() {
  rtc_port_base = add_pio_map(RTC_PORT, 4, rtc_io_handler);
}