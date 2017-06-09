/*
 * Title:   Consio - Windows console library
 * Author:  Matti J. Kärki
 * Date:    2017-06-09
 * Version: 0.3
 * Notes:
 *
 */

#include <tcl.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include "Consio.h"

static HANDLE hStdin;
static HANDLE hStdout;

/*****************************************************************************
 * Consio_Init
 *
 * Description:
 *
 *   This function initializes all commands provided by Consio package.
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   TCL_OK    - initialization was successful
 *   TCL_ERROR - an error message if the initialization procedure was not able
 *               to get console input and output handles
 *
 * Side effects:
 *   Creates a set of new commands for Tcl interpreter under Consio namespace.
 *****************************************************************************/

int Consio_Init(Tcl_Interp *interp) {
    Tcl_Obj *obj_str;
    char *error_message_text = "Can't get standard input or output handle.";

#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, "8.4", 0) == NULL) {
        return TCL_ERROR;
    }
#endif /*USE_TCL_STUBS*/

    Tcl_CreateObjCommand(interp, "Consio::about", cmd_about, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::clrscr", cmd_clrscr, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::gotoxy", cmd_gotoxy, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::wherex", cmd_wherex, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::wherey", cmd_wherey, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::bufferwidth", cmd_bufferwidth, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::bufferheight", cmd_bufferheight, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::getch", cmd_getch, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::getche", cmd_getche, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::putch", cmd_putch, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::kbhit", cmd_kbhit, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::textattr", cmd_textattr, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::cputs", cmd_cputs, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::cgets", cmd_cgets, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::cgetse", cmd_cgetse, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::getchex", cmd_getchex, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::getkeystate", cmd_getkeystate, NULL, NULL);
    Tcl_CreateObjCommand(interp, "Consio::getch2", cmd_getch2, NULL, NULL);

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hStdin == INVALID_HANDLE_VALUE || hStdout == INVALID_HANDLE_VALUE) {
        obj_str = Tcl_NewStringObj(error_message_text,
                                   strlen(error_message_text));
        Tcl_SetObjResult(interp, obj_str);
        return TCL_ERROR;
    }

    return TCL_OK;
}

/*****************************************************************************
 * Consio::about
 *
 * Description:
 *
 *   Returns information about the author and the library version.
 *
 * This command calls the following Windows API functions:
 *
 *   None.
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns a string of information.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_about(ClientData clientData,
                     Tcl_Interp *interp,
                     int objc,
                     Tcl_Obj * CONST objv[]) {
    Tcl_Obj *obj_str;

    obj_str = Tcl_NewStringObj(ABOUT_STRING, strlen(ABOUT_STRING));
    Tcl_SetObjResult(interp, obj_str);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::clrscr
 *
 * Description:
 *
 *   Clears the console using the currently active text attributes.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleScreenBufferInfo
 *   - FillConsoleOutputCharacter
 *   - FillConsoleOutputAttribute
 *   - SetConsoleCursorPosition
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   None.
 *
 * Side effects:
 *
 *   Clears screen i.e. fills console buffer with space character (' ') using
 *   currently selected text attributes (for more information about attributes,
 *   see cmd_textattr function).
 *****************************************************************************/

static int cmd_clrscr(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD home = {0, 0};
    DWORD bufsize;
    DWORD num;

    GetConsoleScreenBufferInfo(hStdout, &info);
    bufsize = info.dwSize.X * info.dwSize.Y;
    FillConsoleOutputCharacter(hStdout, (TCHAR) ' ', bufsize, home, &num);
    GetConsoleScreenBufferInfo(hStdout, &info);
    FillConsoleOutputAttribute(hStdout, info.wAttributes, bufsize, home, &num);
    SetConsoleCursorPosition(hStdout, home);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::gotoxy
 *
 * Description:
 *
 *    Moves cursor to a new location. The location is given as two separate
 *    integer arguments: x and y. 0 0 is the upper left corner of the buffer.
 *
 * This command calls the following Windows API functions:
 *
 *   - SetConsoleCursorPosition
 *
 * Parameters:
 *
 *   x - X coordinate for cursor
 *   y - Y coordinate for cursor
 *
 * Results:
 *
 *   None.
 *
 * Side effects:
 *
 *   Moves the cursor to new location (x;y).
 *****************************************************************************/

static int cmd_gotoxy(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    COORD coord;
    int x, y;

    if (objc < 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "x y");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &x) == TCL_OK &&
        Tcl_GetIntFromObj(interp, objv[2], &y) == TCL_OK) {
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(hStdout, coord);
    }
    else {
        return TCL_ERROR;
    }

    return TCL_OK;
}

/*****************************************************************************
 * Consio::wherex
 *
 * Description:
 *
 *   This command returns the current cursor X coordinate in the console
 *   buffer. This means that the coordinate does not represent the position
 *   of the cursor in the visible area of the console.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleScreenBufferInfo
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 * Returns the current cursor X coordinate as an integer.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_wherex(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    Tcl_Obj *obj_int;

    GetConsoleScreenBufferInfo(hStdout, &info);
    obj_int = Tcl_NewIntObj(info.dwCursorPosition.X);
    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::wherey
 *
 * Description:
 *
 *   This command returns the current cursor Y coordinate in the console
 *   buffer. This means that the coordinate does not represent the position
 *   of the cursor in the visible area of the console.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleScreenBufferInfo
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 * Returns the current cursor Y coordinate as an integer.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_wherey(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    Tcl_Obj *obj_int;

    GetConsoleScreenBufferInfo(hStdout, &info);
    obj_int = Tcl_NewIntObj(info.dwCursorPosition.Y);
    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::bufferwidth
 *
 * Description:
 *
 *   Returns the width of the console buffer. This is not the width of the
 *   console window.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleScreenBufferInfo
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns buffer width as an integer.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_bufferwidth(ClientData clientData,
                           Tcl_Interp *interp,
                           int objc,
                           Tcl_Obj * CONST objv[]) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    Tcl_Obj *obj_int;

    GetConsoleScreenBufferInfo(hStdout, &info);
    obj_int = Tcl_NewIntObj(info.dwSize.X);
    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::bufferheight
 *
 * Description:
 *
 *   Returns the height of the console buffer. This is not the height of the
 *   console window.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleScreenBufferInfo
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns buffer height as an integer.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_bufferheight(ClientData clientData,
                            Tcl_Interp *interp,
                            int objc,
                            Tcl_Obj * CONST objv[]) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    Tcl_Obj *obj_int;

    GetConsoleScreenBufferInfo(hStdout, &info);
    obj_int = Tcl_NewIntObj(info.dwSize.Y);
    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::getch
 *
 * Description:
 *
 *   Waits for a keypress. Doesn't echo it to the console.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleMode
 *   - SetConsoleMode
 *   - ReadConsole
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns the first character from the input buffer.
 *
 * Side effects:
 *
 *   If there are no characters available in the input buffer, the function
 *   will block and wait until user presses a key. The pressed key will not
 *   be echoed back to the console.
 *****************************************************************************/

static int cmd_getch(ClientData clientData,
                     Tcl_Interp *interp,
                     int objc,
                     Tcl_Obj * CONST objv[]) {
    DWORD oldMode, newMode;
    TCHAR buffer[1];
    DWORD num;
    Tcl_Obj *obj_str;

    newMode = 0;

    GetConsoleMode(hStdin, &oldMode);
    SetConsoleMode(hStdin, newMode);
    ReadConsole(hStdin, buffer, 1, &num, NULL);
    SetConsoleMode(hStdin, oldMode);

    if (num > 0) {
        obj_str = Tcl_NewStringObj(buffer, 1);
        Tcl_SetObjResult(interp, obj_str);
    }

    return TCL_OK;
}

/*****************************************************************************
 * Consio::getche
 *
 * Description:
 *
 *   Waits for a keypress. The character received from the keyboard will be
 *   echoed back to the console.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleMode
 *   - SetConsoleMode
 *   - ReadConsole
 *   - WriteConsole
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns the first character from the input buffer.
 *
 * Side effects:
 *
 *   If there are no characters available in the input buffer, the function
 *   will block and wait until user presses a key. The pressed key will be
 *   echoed back to the console.
 *****************************************************************************/

static int cmd_getche(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    DWORD oldMode, newMode;
    TCHAR buffer[1];
    DWORD num;
    Tcl_Obj *obj_str;

    newMode = 0;

    GetConsoleMode(hStdin, &oldMode);
    SetConsoleMode(hStdin, newMode);
    ReadConsole(hStdin, buffer, 1, &num, NULL);
    SetConsoleMode(hStdin, oldMode);

    if (num > 0) {
        WriteConsole(hStdout, buffer, 1, &num, NULL);

        obj_str = Tcl_NewStringObj(buffer, 1);
        Tcl_SetObjResult(interp, obj_str);
    }

    return TCL_OK;
}

/*****************************************************************************
 * Consio::putch
 *
 * Description:
 *
 *    Prints the given character to the current cursor location.
 *
 * This command calls the following Windows API functions:
 *
 *   - WriteConsole
 *
 * Parameters:
 *
 *   chr - character to be displayed.
 *
 * Results:
 *
 *   None.
 *
 * Side effects:
 *
 *   Given character will be displayed at the cursor location.
 *****************************************************************************/

static int cmd_putch(ClientData clientData,
                     Tcl_Interp *interp,
                     int objc,
                     Tcl_Obj * CONST objv[]) {
    DWORD num;
    char *str;

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "char");

        return TCL_ERROR;
    }

    str = Tcl_GetString(objv[1]);
    WriteConsole(hStdout, str, 1, &num, NULL);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::kbhit
 *
 * Description:
 *
 *    With this command it's possible to check if the user has pressed any
 *    keys. This command will not block, so it's possible to poll for
 *    keys in a while loop like this: while {[Consio::kbhit] == 0} {}.
 *
 * This command calls the following Windows API functions:
 *
 *   - PeekConsoleInput
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   1 - if there is at least one character waiting in the input buffer
 *   0 - if there is nothing waiting in the input buffer
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_kbhit(ClientData clientData,
                     Tcl_Interp *interp,
                     int objc,
                     Tcl_Obj * CONST objv[]) {
    INPUT_RECORD buffer[1 * sizeof(TCHAR)];
    DWORD num;
    Tcl_Obj *obj_int;

    PeekConsoleInput(hStdin, buffer, 1, &num);

    if (num == 0) {
        obj_int = Tcl_NewIntObj(0);
    }
    else {
        obj_int = Tcl_NewIntObj(1);
    }

    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::textattr
 *
 * Description:
 *
 *   This command will change the console attributes, which will be
 *   used for all output. The command has two parameters: foreground
 *   color name and background color name. The available color names
 *   are: black, blue, green, cyan, red, magenta, brown, lightgray,
 *   darkgray, lightblue, lightgreen, lightcyan, lightred, lightmagenta,
 *   yellow and white.
 *
 * This command calls the following Windows API functions:
 *
 *   - SetConsoleTextAttribute
 *
 * Parameters:
 *
 *   foreground - a new foreground color to be used
 *   background - a new background color to be used
 *
 * Results:
 *
 *   None.
 *
 * Side effects:
 *
 *   Nothing visible until the first character is printed to the console.
 *   New attributes are used for everything until the console is destroyed
 *   or something will call the SetConsoleTextAttribute Win API function
 *   with new attributes. In other words, the new attributes will stay in
 *   effect even after the Tcl application has terminated and the Consio
 *   library is not used anymore.
 *****************************************************************************/

static int cmd_textattr(ClientData clientData,
                        Tcl_Interp *interp,
                        int objc,
                        Tcl_Obj * CONST objv[]) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    WORD attr = 0;
    int f, b;
    CONST char *colors[] = {"black",     "blue",        "green",
                            "cyan",      "red",         "magenta",
                            "brown",     "lightgray",   "darkgray",
                            "lightblue", "lightgreen",  "lightcyan",
                            "lightred",  "lightmagenta", "yellow",
                            "white", (char *) NULL};
    int index;

    if (objc < 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "foreground background");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp,
                            objv[1],
                            colors,
                            "foreground color",
                            0,
                            &index) == TCL_OK) {
        switch (index) {
            case BLACK       : attr = attr | FG_BLACK;        break;
            case BLUE        : attr = attr | FG_BLUE;         break;
            case GREEN       : attr = attr | FG_GREEN;        break;
            case CYAN        : attr = attr | FG_CYAN;         break;
            case RED         : attr = attr | FG_RED;          break;
            case MAGENTA     : attr = attr | FG_MAGENTA;      break;
            case BROWN       : attr = attr | FG_BROWN;        break;
            case LIGHTGRAY   : attr = attr | FG_LIGHTGRAY;    break;
            case DARKGRAY    : attr = attr | FG_DARKGRAY;     break;
            case LIGHTBLUE   : attr = attr | FG_LIGHTBLUE;    break;
            case LIGHTGREEN  : attr = attr | FG_LIGHTGREEN;   break;
            case LIGHTCYAN   : attr = attr | FG_LIGHTCYAN;    break;
            case LIGHTRED    : attr = attr | FG_LIGHTRED;     break;
            case LIGHTMAGENTA: attr = attr | FG_LIGHTMAGENTA; break;
            case YELLOW      : attr = attr | FG_YELLOW;       break;
            case WHITE       : attr = attr | FG_WHITE;        break;
            default: return TCL_OK;
        }
    }
    else {
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp,
                            objv[2],
                            colors,
                            "background color",
                            0,
                            &index) == TCL_OK) {
        switch (index) {
            case BLACK       : attr = attr | BG_BLACK;        break;
            case BLUE        : attr = attr | BG_BLUE;         break;
            case GREEN       : attr = attr | BG_GREEN;        break;
            case CYAN        : attr = attr | BG_CYAN;         break;
            case RED         : attr = attr | BG_RED;          break;
            case MAGENTA     : attr = attr | BG_MAGENTA;      break;
            case BROWN       : attr = attr | BG_BROWN;        break;
            case LIGHTGRAY   : attr = attr | BG_LIGHTGRAY;    break;
            case DARKGRAY    : attr = attr | BG_DARKGRAY;     break;
            case LIGHTBLUE   : attr = attr | BG_LIGHTBLUE;    break;
            case LIGHTGREEN  : attr = attr | BG_LIGHTGREEN;   break;
            case LIGHTCYAN   : attr = attr | BG_LIGHTCYAN;    break;
            case LIGHTRED    : attr = attr | BG_LIGHTRED;     break;
            case LIGHTMAGENTA: attr = attr | BG_LIGHTMAGENTA; break;
            case YELLOW      : attr = attr | BG_YELLOW;       break;
            case WHITE       : attr = attr | BG_WHITE;        break;
            default: return TCL_OK;
        }
    }
    else {
        return TCL_ERROR;
    }

    SetConsoleTextAttribute(hStdout, attr);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::cputs
 *
 * Description:
 *
 *   This command is just a reimplementation of the Tcl puts command. This will
 *   print the given string to the console. If -nonewline parameter has been
 *   specified, then the newline will not be appended at the end of the string.
 *   At the moment this command has 65534 character limit. It's better to use
 *   puts istead, if there is a need to print more charactes.
 *
 * This command calls the following Windows API functions:
 *
 *   - WriteConsole
 *
 * Parameters:
 *
 *   -nonewline - (optional) prevents the newline
 *   string     - string to be printed
 *
 * Results:
 *
 *   None.
 *
 * Side effects:
 *
 *   The given string will be printed to the console.
 *****************************************************************************/

static int cmd_cputs(ClientData clientData,
                     Tcl_Interp *interp,
                     int objc,
                     Tcl_Obj * CONST objv[]) {
    TCHAR *str;
    TCHAR *param;
    TCHAR *newline;
    int len;
    DWORD num;
    char *errstring = "?-nonewline? string";

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, objc, objv, errstring);
        return TCL_ERROR;
    }

    if (objc > 2) {
        param = Tcl_GetStringFromObj(objv[1], &len);
        str = Tcl_GetStringFromObj(objv[2], &len);
        if (strcmp(param, "-nonewline") == 0) {
            newline = NULL;
        }
        else {
            Tcl_WrongNumArgs(interp, 1, objv, errstring);
            return TCL_ERROR;
        }
    }
    else {
        str = Tcl_GetStringFromObj(objv[1], &len);
        newline = "\r\n";
    }

    if (len > 65534) len = 65534;
    WriteConsole(hStdout, str, len, &num, NULL);
    if (newline != NULL) WriteConsole(hStdout, newline, 2, &num, NULL);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::cgets
 *
 * Description:
 *
 *   This command reads user input until the Enter key has been pressed. This
 *   command will not echo anything to the console. The maximum input size at
 *   the moment is 4095 characters.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleMode
 *   - SetConsoleMode
 *   - ReadConsole
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns a string. Newline character will not be included.
 *
 * Side effects:
 *
 *   The command will block until the Enter key has been pressed.
 *****************************************************************************/

static int cmd_cgets(ClientData clientData,
                     Tcl_Interp *interp,
                     int objc,
                     Tcl_Obj * CONST objv[]) {
    DWORD oldMode, newMode;
    TCHAR buffer[4096];
    DWORD num;
    Tcl_Obj *obj_str;

    newMode = ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT;
    GetConsoleMode(hStdin, &oldMode);
    SetConsoleMode(hStdin, newMode);
    ReadConsole(hStdin, buffer, 4095, &num, NULL);
    SetConsoleMode(hStdin, oldMode);

    if (buffer[num - 1] == '\r' || buffer[num - 1] == '\n') num--;
    if (buffer[num - 1] == '\r' || buffer[num - 1] == '\n') num--;

    obj_str = Tcl_NewStringObj(buffer, num);
    Tcl_SetObjResult(interp, obj_str);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::cgetse
 *
 * Description:
 *
 *   This command reads user input until the Enter key has been pressed. This
 *   command will echo everything back to the console. The maximum input size
 *   at the moment is 4095 characters.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleMode
 *   - SetConsoleMode
 *   - ReadConsole
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns a string. Newline character will not be included.
 *
 * Side effects:
 *
 *   The command will block until the Enter key has been pressed. The user
 *   input will be echoed to the console.
 *****************************************************************************/

static int cmd_cgetse(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    DWORD oldMode, newMode;
    TCHAR buffer[4096];
    DWORD num;
    Tcl_Obj *obj_str;

    newMode = ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT;
    GetConsoleMode(hStdin, &oldMode);
    SetConsoleMode(hStdin, newMode);
    ReadConsole(hStdin, buffer, 4095, &num, NULL);
    SetConsoleMode(hStdin, oldMode);

    if (buffer[num - 1] == '\r' || buffer[num - 1] == '\n') num--;
    if (buffer[num - 1] == '\r' || buffer[num - 1] == '\n') num--;

    obj_str = Tcl_NewStringObj(buffer, num);
    Tcl_SetObjResult(interp, obj_str);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::getchex
 *
 * Description:
 *
 *   Extended getch will wait for key press from the keyboard and returns it
 *   as a device-independent integer code. This function will react to any
 *   key.
 *
 * This command calls the following Windows API functions:
 *
 *   - GetConsoleMode
 *   - SetConsoleMode
 *   - ReadConsoleInput
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns integer value of the key. The value is device-independent as
 *   opposed to scan code.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_getchex(ClientData clientData,
                       Tcl_Interp *interp,
                       int objc,
                       Tcl_Obj * CONST objv[]) {
    DWORD oldMode, newMode;
    INPUT_RECORD buffer[1];
    DWORD num;
    WORD code;
    Tcl_Obj *obj_str;

    newMode = 0;

    GetConsoleMode(hStdin, &oldMode);
    SetConsoleMode(hStdin, newMode);

    do {
        ReadConsoleInput(hStdin, buffer, 1, &num);
    } while (num == 0 || buffer[0].EventType != KEY_EVENT ||
             buffer[0].Event.KeyEvent.bKeyDown == FALSE);

    SetConsoleMode(hStdin, oldMode);

    code = buffer[0].Event.KeyEvent.wVirtualKeyCode;
    obj_str = Tcl_NewIntObj(code);
    Tcl_SetObjResult(interp, obj_str);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::getkeystate
 *
 * Description:
 *
 * See GetAsyncKeyState from MSDN:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=vs.85).aspx
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
 *
 * This command calls the following Windows API functions:
 *
 *   - GetAsyncKeyState
 *
 * Parameters:
 *
 *   id - virtual-key code
 *
 * Results:
 *
 * Returns key state. The most significant bit tells if the key is up (bit not
 * set) or down (sit set). The lease significant bit tells, if the key was
 * pressed after previous call of getkeystate.
 *
 * Side effects:
 *
 *   None.
 *****************************************************************************/

static int cmd_getkeystate(ClientData clientData,
                           Tcl_Interp *interp,
                           int objc,
                           Tcl_Obj * CONST objv[]) {
    int id;
    int state;
    Tcl_Obj *obj_int;

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "id");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &id) == TCL_OK) {
        state = GetAsyncKeyState(id);
    }
    else {
        return TCL_ERROR;
    }

    obj_int = Tcl_NewIntObj(state);
    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}

/*****************************************************************************
 * Consio::getch2
 *
 * Description:
 *
 *   Waits for a keypress. Doesn't echo it to the console.
 *   This is based on MSVCRT implementation. See more information here:
 *   https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch
 *
 * This command calls the following Windows API functions:
 *
 *   - _getch
 *
 * Parameters:
 *
 *   None.
 *
 * Results:
 *
 *   Returns the first character from the input buffer. If the value is larger
 *   than 255, then the key was a special key (arrow, function key, etc.)
 *
 * Side effects:
 *
 *
 *
 *****************************************************************************/

static int cmd_getch2(ClientData clientData,
                      Tcl_Interp *interp,
                      int objc,
                      Tcl_Obj * CONST objv[]) {
    int key;
    Tcl_Obj *obj_int;

    key = _getch();
    if (key == 0 || key == 0xE0) {
        key = _getch() + 0x100;
    }

    obj_int = Tcl_NewIntObj(key);
    Tcl_SetObjResult(interp, obj_int);

    return TCL_OK;
}
