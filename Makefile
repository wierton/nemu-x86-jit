NAME = nemu
SRCS = $(shell find src/ -name "*.c")
CFLGAS = -D DEBUG
LIBS += klib compiler-rt
include $(AM_HOME)/Makefile.app

