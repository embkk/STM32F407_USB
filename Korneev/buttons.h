#ifndef BUTTONS_H
#define BUTTONS_H

#include "log.h"

#define BTN_PRESS_COUNT 1
#define BTN_CHECK_MS 20

extern uint32_t bound_timer;
extern uint8_t btn_state[3];
extern uint32_t btn_count[3];

void Buttons_init(void);
void LED_init(void);
void Buttons_check(void);

#endif