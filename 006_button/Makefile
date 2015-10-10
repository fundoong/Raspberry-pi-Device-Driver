obj-m=mod.o

OBJECTS = $(patsubst % .c, % .o , $(wildcard *.c))


KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.c .mod.* Module.symvers modules.order 
