#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common.h"
#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
extern FILE* log_fp;
#	define Log_write(format, ...) \
  do { \
    if (log_fp != NULL) { \
      fprintf(log_fp, format, ## __VA_ARGS__); \
    } \
  } while (0)
#else
#	define Log_write(format, ...)
#endif

#define printflog(format, ...) \
  do { \
    printf(format, ## __VA_ARGS__); \
    Log_write(format, ## __VA_ARGS__); \
  } while (0)

#define Log(format, ...) \
    printflog("\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define Assert(cond, ...) \
  do { \
    if (!(cond)) { \
      printf("\33[1;31m"); \
      printf(__VA_ARGS__); \
      printf("\33[0m\n"); \
      assert(cond); \
    } \
  } while (0)

#define panic(format, ...) \
  Assert(0, format, ## __VA_ARGS__)

#define TODO() panic("please implement me")

#endif
