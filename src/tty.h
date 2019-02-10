#ifndef TTY_H
#define TTY_H
#include "printer.h"

printer_t tty_vt100;
page_t tty_80x25;
page_t tty_80xINF;

void tty_init();

#endif // TTY_H
