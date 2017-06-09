# consio
Consio is a console library for Tcl/Tk implementing some basic functionality for making console applications under Windows.

### Latest Binaries

* https://github.com/mattijk/consio/releases/download/0.2/Consio.zip


#### INFORMATION

Consio is a console library for Windows, which implements some basic
functionality for making console applications under Windows environment.

This library has been tested only under Windows XP.


#### LICENSE & CONTACT

Copyright 2005 Matti J. Kärki <mjk@iki.fi>

The license is a BSD License and it has been copied from
http://www.opensource.org/licenses/bsd-license.php

All bug reports and suggestions can be sent to the author via e-mail:
<mjk@iki.fi>.

The current version is 0.2.


#### INSTALLATION

The Consio archive has a compiled version of the library available, so you
don't need to compile anything. However, if you want to compile the library
yourself, there is a Makefile, which has all required information available
for compiling the library under Cygwin. Note, that even though the library
has been compiled using Cygwin tools, it is using MinGW libraries and header
files, so there are no Cygwin-specific dependencies in the library. You will
also need the libtclstub84.a library from MinGW of Cygwin compiled Tcl
distribution (or you can get one from ActiveState Tcl distribution and
rename the tclstub84.lib to libtclstub84.a).

To install this package, just copy the Consio directory and all files under it
to the "lib" directory of your Tcl installation, so the end result should be
something like this:
```
  C:\Tcl\lib\Consio
  C:\Tcl\lib\Consio\pkgIndex.tcl
  C:\Tcl\lib\Consio\Consio.tcl
  C:\Tcl\lib\Consio\Consio.dll
   .
   .
   .
  And so on
```

#### USAGE

In Tcl, just call

  `package require Consio`
  
and the Consio library should load automatically. After that, all commands
are available under the Consio:: namespace.


#### PACKAGE REFERENE

The library includes the following functions:


`Consio::clrscr`

  Clears screen (console buffer).

`Consio::gotoxy x y`

  Sets a new cursor location to (x;y).

`Consio::wherex`

  Returns the X position of the cursor in the console buffer.

`Consio::wherey`

  Returns the Y position of the cursor in the console buffer.

`Consio::bufferwidth`

  Returns the width of the console buffer.

`Consio::bufferheight`

  Returns the height of the console buffer.

`Consio::getch`

  Waits for single key press.

`Consio::getche`

  Waits for single key press and echoes it back to the console.

`Consio::putch chr`

  Inserts a character to the cursor location.

`Consio::kbhit`

  checks if there are any keystokes waiting in the input buffer.
  This function doesn't block or remove anything from the buffer.
  The function will return 1 if there are keystrokes waiting in
  the input buffer. Otherwise returns 0.

`Consio::textattr foreground background`

  Sets foreground and background color. Available color attributes
  for both foreground and background color are:
  
  black blue green cyan red magenta brown lightgray darkgray
  lightblue lightgreen lightcyan lightred lightmagenta yellow white

`Consio::cputs ?-nonewline? string`

  Prints string to standard output. If -nonewline is specified, then
  then the newline will not be printed at the end of the line.

`Consio::cgets`

  Reads keystrokes from the console until Enter key is pressed. Linefeed
  of newline characters are not included in the result.

`Consio::cgetse`

  Reads keystrokes from the console and echoes them back until
  the Enter key is pressed. Linefeed or newline characters are not
  included in the results.

`Consio::getchex`

  Extended getch. Getch is unable to return a meaningful code for all keys.
  For example, there is no easy way to return arrow keys as a single
  character. Getchex works like getch except it will return key presses as
  an integer value. This way it's possible to receive a code from all
  keys, including shifts, function keys, arrows etc. The code is
  device-independent and defined by Windows.
