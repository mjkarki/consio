TCL_PATH	= /opt/tcl
CC		= gcc -mno-cygwin

Consio.dll: Consio.c Consio.h
	$(CC) -DUSE_TCL_STUBS -I$(TCL_PATH)/include -s -shared -o Consio.dll Consio.c $(TCL_PATH)/lib/libtclstub84.a
