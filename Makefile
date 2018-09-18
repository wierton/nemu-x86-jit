NAME = nemu
SRCS = $(shell find src/ -name "*.c")
LIBS += compiler-rt libndl

include $(NAVY_HOME)/Makefile.app
