# NEMU

NEMU(NJU Emulator) is a simple but complete full-system x86 emulator designed for teaching purpose.
Small x86 programs can run under NEMU.

This branch runs on AM (tested on AM native). The guest program is typing game, which is hard coded in
`src/monitor/image.h`. Use the following command to compile and run
```
make ARCH=native run
```


If you want to change the guest program, before compile NEMU, first compile the guest program using AM(ARCH=x86-nemu), 
then run the following command
```
./gen-image.sh /path/to/guest/binary
```
e.g.
```
./gen-image.sh $AM_HOME/tests/videotest/build/videotest-x86-nemu.bin
```
The hexdump of the guest program will be saved in `src/monitor/image.h`. After that, compile and run NEMU as stated above.

----------------------------

The main features of NEMU include
* ~~a small monitor with a simple debugger~~
  * single step
  * register/memory examination
  * expression evaluation without the support of symbols
  * watch point
  * differential testing with QEMU
* CPU core with support of most common used x86 instructions in protected mode
  * real mode is not supported
  * x87 floating point instructions are not supported
* memory
* ~~I386 paging~~
  * TLB is optional
  * protection is not supported
* ~~I386 interrupt and exception~~
  * protection is not supported
* 4 devices
  * serial, timer, keyboard, VGA
  * most of them are simplified and unprogrammable
* 2 types of I/O
  * port-mapped I/O and memory-mapped I/O
