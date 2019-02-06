#ifndef TTY_H
#define TTY_H
char start[] = "";
char nl[]="\n";

printer_t tty_vt100 = {
  .name = "VT100",
  .feed_bit = tty_feed_bit,
}

page_t tty_80x25 = {
  .width=80,
  .height=25
};

void tty_init();

#endif // TTY_H
