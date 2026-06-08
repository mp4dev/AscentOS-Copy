.POSIX:

kernel/serial.o: kernel/serial.c include/mpx/io.h include/mpx/serial.h \
  include/mpx/device.h include/sys_req.h

kernel/kmain.o: kernel/kmain.c include/mpx/gdt.h include/mpx/interrupts.h \
  include/mpx/serial.h include/mpx/device.h include/mpx/vm.h \
  include/sys_req.h include/string.h include/memory.h

kernel/core-c.o: kernel/core-c.c include/mpx/gdt.h include/mpx/panic.h \
  include/mpx/interrupts.h include/mpx/io.h include/mpx/serial.h \
  include/mpx/device.h include/sys_req.h include/string.h \
  include/mpx/vm.h

kernel/functions.o: kernel/functions.c include/string.h \
  include/mpx/functions.h

KERNEL_OBJECTS=\
	kernel/core-asm.o\
	kernel/sys_call_isr.o\
	kernel/serial.o\
	kernel/kmain.o\
	kernel/core-c.o\
  kernel/functions.o\
  kernel/pcb.o\
  kernel/color.o\
  kernel/print.o\
  kernel/sys_call.o\
  kernel/alarm.o\
  kernel/mcb.o\
  kernel/iocb.o\
  kernel/dcb.o\
  kernel/devicedriver.o\
  kernel/serial_isr.o
