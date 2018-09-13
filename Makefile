NAME = nemu
SRCS = $(shell find src/ -name "*.c")
LIBS += compiler-rt

include $(NAVY_HOME)/Makefile.app
