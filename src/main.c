#include "common.h"

int init_monitor(int, char *[]);
// void ui_mainloop(int);

void cpu_exec(uint64_t n);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  // int is_batch_mode = init_monitor(argc, argv);
  init_monitor(argc, argv);

  /* Receive commands from user. */
  // ui_mainloop(is_batch_mode);

  cpu_exec(-1);

  return 0;
}
