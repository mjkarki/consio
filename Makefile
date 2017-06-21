TCL_ROOT	= /opt
TCL_85_32	= $(TCL_ROOT)/tcl8.5-win32
TCL_85_64	= $(TCL_ROOT)/tcl8.5-win64
TCL_86_32	= $(TCL_ROOT)/tcl8.6-win32
TCL_86_64	= $(TCL_ROOT)/tcl8.6-win64
CC32		= i686-w64-mingw32-gcc
CC64		= x86_64-w64-mingw32-gcc

all: Consio.dll

Consio.dll: Consio.c Consio.h
	mkdir -p bin/8.5/32bit/
	mkdir -p bin/8.5/64bit/
	mkdir -p bin/8.6/32bit/
	mkdir -p bin/8.6/64bit/
	$(CC32) -DTCLVERSION=\"8.5\" -DUSE_TCL_STUBS -I$(TCL_85_32)/include -s -shared -o bin/8.5/32bit/Consio.dll Consio.c $(TCL_85_32)/lib/libtclstub85.a
	$(CC64) -DTCLVERSION=\"8.5\" -DUSE_TCL_STUBS -I$(TCL_85_64)/include -s -shared -o bin/8.5/64bit/Consio.dll Consio.c $(TCL_85_64)/lib/libtclstub85.a
	$(CC32) -DTCLVERSION=\"8.6\" -DUSE_TCL_STUBS -I$(TCL_86_32)/include -s -shared -o bin/8.6/32bit/Consio.dll Consio.c $(TCL_86_32)/lib/libtclstub86.a
	$(CC64) -DTCLVERSION=\"8.6\" -DUSE_TCL_STUBS -I$(TCL_86_64)/include -s -shared -o bin/8.6/64bit/Consio.dll Consio.c $(TCL_86_64)/lib/libtclstub86.a

clean:
	rm -rf bin
