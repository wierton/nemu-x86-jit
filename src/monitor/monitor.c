#include "nemu.h"
#include "image.h"
// #include <unistd.h>

#define ENTRY_START 0x100000

// void init_difftest();
// void init_regex();
// void init_wp_pool();
void init_device();

void reg_test();
// void init_qemu_reg();
// bool gdb_memcpy_to_qemu(uint32_t, void *, int);

// FILE *log_fp = NULL;
// static char *log_file = NULL;
static char *img_file = NULL;
// static int is_batch_mode = false;

// static inline void init_log() {
// #ifdef DEBUG
//   if (log_file == NULL) return;
//   log_fp = fopen(log_file, "w");
//   Assert(log_fp, "Can not open '%s'", log_file);
// #endif
// }

static inline void welcome() {
  printf("Welcome to NEMU!\n");
  Log("Build time: %s, %s", __TIME__, __DATE__);

#ifdef DEBUG
  Log("Debug: \33[1;32m%s\33[0m", "ON");
  Log("If debug mode is on, A log file will be generated to record every instruction NEMU executes. "
      "This may lead to a large log file. "
      "If it is not necessary, you can turn it off in include/common.h.");
#else
  Log("Debug: \33[1;32m%s\33[0m", "OFF");
#endif

#ifdef DIFF_TEST
  Log("Differential testing: \33[1;32m%s\33[0m", "ON");
  Log("If differential testing mode is on, the result of every instruction will be compared with QEMU. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in include/common.h.");
#else
  Log("Differential testing: \33[1;32m%s\33[0m", "OFF");
#endif

  // printf("For help, type \"help\"\n");
}

static inline int load_default_img() {
  Log("No image is given. Use the default build-in image.");

  memcpy(guest_to_host(ENTRY_START), img, sizeof(img));

  return sizeof(img);
}

static inline void load_image() {
  long size;
  if (img_file == NULL) {
    size = load_default_img();
  } else {
    int ret;

    FILE *fp = fopen(img_file, "rb");
    Assert(fp, "Can not open '%s'", img_file);

    Log("The image is %s", img_file);

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    ret = fread(guest_to_host(ENTRY_START), size, 1, fp);
    assert(ret == 1);

    fclose(fp);
  }

#ifdef DIFF_TEST
  gdb_memcpy_to_qemu(ENTRY_START, guest_to_host(ENTRY_START), size);
#endif
}

static inline void restart() {
  /* Set the initial instruction pointer. */
  cpu.eip = ENTRY_START;

#ifdef DIFF_TEST
  init_qemu_reg();
#endif

  /* Initialize EFLAGS register */
  cpu.eflags.val = 0x2;
}

// static inline void parse_args(int argc, char *argv[]) {
//   int o;
//   while ( (o = getopt(argc, argv, "-bl:")) != -1) {
//     switch (o) {
//       case 'b': is_batch_mode = true; break;
//       case 'l': log_file = optarg; break;
//       case 1:
//                 if (img_file != NULL) Log("too much argument '%s', ignored", optarg);
//                 else img_file = optarg;
//                 break;
//       default:
//                 panic("Usage: %s [-b] [-l log_file] [img_file]", argv[0]);
//     }
//   }
// }

int init_monitor(int argc, char *argv[]) {
  /* Perform some global initialization. */
  if(argc > 0) {
	img_file = argv[1];
  } else {
	img_file = NULL;
  }

  /* Parse arguments. */
  // parse_args(argc, argv);

  /* Open the log file. */
  // init_log();

  /* Test the implementation of the `CPU_state' structure. */
  reg_test();

#ifdef DIFF_TEST
  /* Fork a child process to perform differential testing. */
  init_difftest();
#endif

  /* Load the image to memory. */
  load_image();

  /* Initialize this virtual computer system. */
  restart();

  /* Compile the regular expressions. */
  // init_regex();

  /* Initialize the watchpoint pool. */
  // init_wp_pool();

  /* Initialize devices. */
  init_device();

  /* Display welcome message. */
  welcome();

  return true;
}
