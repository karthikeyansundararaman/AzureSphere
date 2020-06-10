#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

#include "applibs_versions.h"

#include <applibs/gpio.h>
#include <applibs/log.h>

#include "botdrive.h"
#include "hw/avnet_mt3620_sk.h"

#define BOTDRIVE_PERIOD_SECONDS 0
#define BOTDRIVE_PERIOD_NANO_SECONDS 2000000

static int botdrivePollTimerFd = -1;
extern int botdrive_M1_A_GpioFd;
extern int botdrive_M1_B_GpioFd;
extern int botdrive_M2_A_GpioFd;
extern int botdrive_M2_B_GpioFd;

extern int epollFd;
extern int botdrivestate;
extern volatile sig_atomic_t terminationRequired;


static void botdriveTimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(botdrivePollTimerFd) != 0)
    {
        terminationRequired = true;
        return;
    }

    switch (botdrivestate)
    {
    case 0:
       
        GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_Low);
        break;

    case 1:

        GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_High);
        GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_High);
        break;

    case 2:
        
        GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_High);
        GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_High);
        GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_Low);
        break;

    case 3:

        GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_High);
        GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_High);
        GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_Low);
        break;

    case 4:

        GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_High);
        GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_Low);
        GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_High);
        break;

    }
}



int initBotDrive(void) {

    botdrive_M1_A_GpioFd = GPIO_OpenAsOutput(AVNET_MT3620_SK_GPIO31, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (botdrive_M1_A_GpioFd < 0)
    {
        Log_Debug("ERROR: Could not open botdrive_M1_A GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }

    botdrive_M1_B_GpioFd = GPIO_OpenAsOutput(AVNET_MT3620_SK_GPIO32, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (botdrive_M1_B_GpioFd < 0)
    {
        Log_Debug("ERROR: Could not open botdrive_M1_B GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }

    botdrive_M2_A_GpioFd = GPIO_OpenAsOutput(AVNET_MT3620_SK_GPIO33, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (botdrive_M2_A_GpioFd < 0)
    {
        Log_Debug("ERROR: Could not open botdrive_M2_A GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }

    botdrive_M2_B_GpioFd = GPIO_OpenAsOutput(AVNET_MT3620_SK_GPIO34, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (botdrive_M2_B_GpioFd < 0)
    {
        Log_Debug("ERROR: Could not open botdrive_M2_B GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }

    struct timespec botdriveCheckPeriod = { BOTDRIVE_PERIOD_SECONDS,BOTDRIVE_PERIOD_NANO_SECONDS };
    
    static EventData botdriveEventData = { .eventHandler = &botdriveTimerEventHandler };

    botdrivePollTimerFd = CreateTimerFdAndAddToEpoll(epollFd, &botdriveCheckPeriod, &botdriveEventData, EPOLLIN);
    if (botdrivePollTimerFd < 0)
    {
        return -1;
    }

   return 0;
}


void closeBotDrive(void) {

    CloseFdAndPrintError(botdrive_M1_A_GpioFd, "botdrive_M1_A_Fd");
    CloseFdAndPrintError(botdrive_M1_B_GpioFd, "botdrive_M1_B_Fd");
    CloseFdAndPrintError(botdrive_M2_A_GpioFd, "botdrive_M2_A_Fd");
    CloseFdAndPrintError(botdrive_M2_B_GpioFd, "botdrive_M2_B_Fd");
    CloseFdAndPrintError(botdrivePollTimerFd, "botdrive_Timer_Fd");
}