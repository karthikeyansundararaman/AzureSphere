#pragma once

#include <stdbool.h>
#include "epoll_timerfd_utilities.h"

int initUltrasonicSensor(void);
void closeUltrasonicSensor(void);

extern float cmsFront;
extern float cmsLeft;
extern float cmsRight;
extern float cmsRear;