#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "public.h"

//��ɫ
#define UI_RB     0   //������ɫ
#define UI_YELLOW 1
#define UI_GREEN  2
#define UI_ORANGE 3
#define UI_PURPLE 4
#define UI_PINK   5
#define UI_CYAN   6   //��ɫ
#define UI_BLACK  7
#define UI_WHITE  8




extern u8  draw_cnt;
extern u8  draw_int;
void Client_send_handle(void);
void delete_Coverage(u8 coverage);
extern float pitch_remain;
extern float Yaw_remain;
extern double NX_time;
extern double auto_clck;
extern uint8_t  ddata[66];
extern uint8_t  tx_buf[150];
#endif
