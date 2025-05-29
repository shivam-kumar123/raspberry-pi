obj-m := shivam_module.o

KBD := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KBD) M=$(PWD)  modules

clean:
	make -C $(KBD) M=$(PWD) clean
