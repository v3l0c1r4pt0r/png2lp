char start[] = "\eK\x23\x01";
char nl[]="\r\eJ\x18";

printer_t oki_3321 = {
  .name = "Oki 3321",
  .feed_bit = oki_feed_bit,
}

page_t oki_a4p = {
  .name = "A4 Portrait",
  .width=0x1e4,
  .height=95*8
};
