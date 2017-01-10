#include "led.h"
void send_bit0();
void send_bit1();
void reset();
void send_Green();
void send_Red();
void send_Blue();
void send_GRB(u8 Green,u8 Red,u8 Blue);
void send24_GRB(u8 Green,u8 Red,u8 Blue);
void double_GRB();
void sendi_GRB(u8 Green,u8 Red,u8 Blue,u8 j);
void send_8G_8R_8B();
void send_4G_4R_4B_1();
void send_4G_4R_4B_2();
void send_semicircle();
void send_circle();
void send_circle_cycle(u8 cycle,u8 delay_time);