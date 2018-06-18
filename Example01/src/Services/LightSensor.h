
#ifndef SERVICES_LIGHTSENSOR_H_
#define SERVICES_LIGHTSENSOR_H_

#include <stdint.h>
#include "common.h"

void LightSensor_init ();
void LightSensor_setCallback(callback_t c);
int32_t LightSensor_read(void);
int32_t LightSensor_getLight(void);

#endif
