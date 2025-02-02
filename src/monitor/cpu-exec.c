#include <inttypes.h>

#include "nemu.h"
#include "monitor/monitor.h"
//#include "monitor/watchpoint.h"
#include "cpu/tb.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = NEMU_STOP;

bool exec_wrapper(bool);

static uint64_t g_nr_guest_instr = 0;

void nr_guest_instr_add(uint32_t n) {
  g_nr_guest_instr += n;
}

static int tot_tb = 0, new_tb = 0;
static uint64_t nr_rtl_instr = 0;

static TranslationBlock *tblocks = NULL;
TranslationBlock *cur_tblock = NULL;

int exec_time_sort(const TranslationBlock *a, const TranslationBlock *b) {
	return a->exec_time < b->exec_time;
}

void monitor_statistic() {
  Log("total guest instructions = %" PRIu64, g_nr_guest_instr);
  Log("total RTL instructions = %" PRIu64, nr_rtl_instr);
//  Log("#rtl/#guest-instr = %lf", (double)nr_rtl_instr/g_nr_guest_instr);
//  Log("Translation block hit rate: %f%%", 100.0 * (tot_tb - new_tb) / tot_tb);

//  HASH_SORT(tblocks, exec_time_sort);
// 
//  TranslationBlock *tb;
//  int cnt = 0;
//  for (tb = tblocks; tb != NULL && cnt < 10; tb = tb->hh.next, ++cnt) {
//     printf("eip: [0x%x, 0x%x) #guest-instr: %d, #rtl-instr: %d, #exec: %d\n",
//   		  tb->eip_start, tb->eip_end, 
//   		  tb->guest_instr_cnt, tb->rtl_instr_cnt,
//   		  tb->exec_time);
//  }
//  printf("\n");
//  cnt = 0;
//  for (tb = tblocks; tb != NULL && cnt < 10; tb = tb->hh.next, ++cnt) {
//     Log("eip: [0x%x, 0x%x) #guest-instr: %d, #rtl-instr: %d, #exec: %d",
//   		  tb->eip_start, tb->eip_end, 
//   		  tb->guest_instr_cnt, tb->rtl_instr_cnt,
//   		  tb->exec_time);
//     print_tblock(tb);
//   
//  }

}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  if (nemu_state == NEMU_END || nemu_state == NEMU_ABORT) {
    printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
    return;
  }
  nemu_state = NEMU_RUNNING;

  bool print_flag = n < MAX_INSTR_TO_PRINT;

  for (; n > 0; n --) {
#if defined(DIFF_TEST)
	vaddr_t ori_eip = cpu.eip;
#endif

	/* Translate one basic block if necessary, then execute it */

	// search the translation block beginning at eip
	HASH_FIND_INT(tblocks, &cpu.eip, cur_tblock);

	if (cur_tblock == NULL) {
		// translate a block and save RTLs
		cur_tblock = (TranslationBlock *)malloc(sizeof(TranslationBlock));
		cur_tblock->eip_start = cpu.eip;
		cur_tblock->guest_instr_cnt = 0;
		cur_tblock->rtl_instr_cnt = 0;
		cur_tblock->has_saved_label = 0;
		cur_tblock->exec_time = 0;
		INIT_LIST_HEAD(&cur_tblock->rtl_instr_list.list);
		HASH_ADD_INT(tblocks, eip_start, cur_tblock);

		// translate up to (and include) next control transfer instr
		do {
			++cur_tblock->guest_instr_cnt;
		}while(!exec_wrapper(print_flag));
//		++cur_tblock->guest_instr_cnt;
//		exec_wrapper(print_flag);

		cur_tblock->eip_end = cpu.eip;

		optimize_tblock(cur_tblock);

		++new_tb;
//		Log("New translation block. eip: [0x%x, 0x%x) #guest-instr: %d, $rtl-instr: %d",
//				cur_tblock->eip_start, cur_tblock->eip_end, 
//				cur_tblock->guest_instr_cnt, cur_tblock->rtl_instr_cnt);
//		print_tblock(cur_tblock);
	}
//  else
//		Log("Hit translation block. eip_start: 0x%x, eip_end: 0x%x, #instr: %d",
//				cur_tblock->eip_start, cur_tblock->eip_end, cur_tblock->guest_instr_cnt);

	// execute a basic block
	cpu.eip = cur_tblock->eip_end;
	interpret_tblock(cur_tblock);
    nr_guest_instr_add(cur_tblock->guest_instr_cnt);
	nr_rtl_instr += cur_tblock->rtl_instr_cnt;
	++cur_tblock->exec_time;
	++tot_tb;

#ifdef DIFF_TEST
  void difftest_step(uint32_t, int);
  difftest_step(ori_eip, cur_tblock->guest_instr_cnt);
#endif

#ifdef DEBUG
    /* TODO: check watchpoints here. */
	check_watchpoints();	

#endif

#ifdef HAS_IOE
    // extern void device_update();
    // device_update();
#endif

    if (nemu_state != NEMU_RUNNING) {
      if (nemu_state == NEMU_END) {
        printflog("\33[1;31mnemu: HIT %s TRAP\33[0m at eip = 0x%08x\n\n",
            (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip - 1);
        monitor_statistic();
      }
      else if (nemu_state == NEMU_ABORT) {
        printflog("\33[1;31mnemu: ABORT\33[0m at eip = 0x%08x\n\n", cpu.eip);
      }
	  return;
    }
  }

  if (nemu_state == NEMU_RUNNING) { nemu_state = NEMU_STOP; }
}
