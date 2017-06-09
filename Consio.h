/*
 * Title:   Consio - Windows console library
 * Author:  Matti J. Kärki
 * Date:    2005-10-12
 * Version: 0.2
 * Notes:
 */

#ifndef __Consio_H__
#define __Consio_H__

#define ABOUT_STRING "Consio 0.2 Copyright 2005 Matti J. Kärki <mjk@iki.fi>"

/* Color numbers */

#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15

/* Attributes for foreground colors */

#define FG_BLACK        0
#define FG_BLUE         (FOREGROUND_BLUE)
#define FG_GREEN        (FOREGROUND_GREEN)
#define FG_CYAN         (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_RED          (FOREGROUND_RED)
#define FG_MAGENTA      (FOREGROUND_RED | FOREGROUND_BLUE)
#define FG_BROWN        (FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_LIGHTGRAY    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_DARKGRAY     FOREGROUND_INTENSITY
#define FG_LIGHTBLUE    (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FG_LIGHTGREEN   (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FG_LIGHTCYAN    (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FG_LIGHTRED     (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FG_LIGHTMAGENTA (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FG_YELLOW       (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FG_WHITE        (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)

/* Attributes for background colors */

#define BG_BLACK        0
#define BG_BLUE         (BACKGROUND_BLUE)
#define BG_GREEN        (BACKGROUND_GREEN)
#define BG_CYAN         (BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_RED          (BACKGROUND_RED)
#define BG_MAGENTA      (BACKGROUND_RED | BACKGROUND_BLUE)
#define BG_BROWN        (BACKGROUND_RED | BACKGROUND_GREEN)
#define BG_LIGHTGRAY    (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BG_DARKGRAY     BACKGROUND_INTENSITY
#define BG_LIGHTBLUE    (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define BG_LIGHTGREEN   (BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define BG_LIGHTCYAN    (BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define BG_LIGHTRED     (BACKGROUND_RED | BACKGROUND_INTENSITY)
#define BG_LIGHTMAGENTA (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define BG_YELLOW       (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define BG_WHITE        (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

/* Function definitions */

static int cmd_about(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_clrscr(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_gotoxy(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_wherex(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_wherey(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_bufferwidth(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_bufferheight(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_getch(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_getche(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_putch(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_kbhit(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_textattr(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_cputs(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_cgets(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_cgetse(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);
static int cmd_getchex(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj * CONST objv[]);

#endif /*__Consio_H__*/
