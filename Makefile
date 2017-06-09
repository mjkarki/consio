TCL_PATH	= /opt
CC		= i686-w64-mingw32-gcc

Consio.dll: Consio.c Consio.h
	$(CC) -DUSE_TCL_STUBS -I$(TCL_PATH)/include -s -shared -o Consio.dll Consio.c $(TCL_PATH)/lib/libtclstub86.a
