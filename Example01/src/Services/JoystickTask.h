
#ifndef SERVICES_JOYSTICK_H_
#define SERVICES_JOYSTICK_H_

#include <stdint.h>
#include "common.h"

void Joystick_init(void);
void Joystick_setCallback(callback_t c);

#endif
