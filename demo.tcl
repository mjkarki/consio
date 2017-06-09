lappend auto_path .

package require Consio

set colors {black blue green cyan red magenta brown lightgray darkgray lightblue lightgreen lightcyan lightred lightmagenta yellow white}

::Consio::clrscr
::Consio::cputs -nonewline {Enter your name: }
set name [::Consio::cgetse]
::Consio::cputs -nonewline {Enter some invisible text: }
set str [::Consio::cgets]
::Consio::textattr lightgray black
::Consio::clrscr

for {set x 5} {$x < 21} {incr x 1} {
    for {set y 3} {$y < 19} {incr y 1} {
        ::Consio::gotoxy $x $y
        ::Consio::textattr [lindex $colors [expr $x - 5]] [lindex $colors [expr $y - 3]]
        ::Consio::putch X
    }
}

::Consio::gotoxy 2 20
::Consio::textattr lightgray black
::Consio::cputs -nonewline "$name, you entered the following text -->"
::Consio::cputs -nonewline $str
::Consio::cputs "<--"
::Consio::gotoxy 2 22
::Consio::textattr lightblue black
::Consio::cputs -nonewline {Press any key...}
::Consio::textattr lightgray black
::Consio::getchex
::Consio::gotoxy 0 0
