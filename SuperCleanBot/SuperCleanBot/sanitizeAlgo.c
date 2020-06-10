#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

#include <applibs/gpio.h>
#include <applibs/log.h>

#include "hw/avnet_mt3620_sk.h"
#include "oled.h"
#include "sanitizeAlgo.h"


#define SANITIZEALGO_PERIOD_SECONDS 0
#define SANITIZEALGO_PERIOD_NANO_SECONDS 500000000

#define SANITIZEALGO_PERIOD_ELAPSED_SECONDS 1
#define SANITIZEALGO_PERIOD_ELAPSED_NANO_SECONDS 0

static int sanitizeAlgoPollTimerFd = -1;

static int sanitizeAlgoPollElapsedTimerFd = -1;


//Critical Distance Limit - 10cm
static int criticalDLimit = 10;

//Safe Distance Limit - 20cm
static int safeDLimit = 20;

static int CorrectionInprogressFlag = 0;

extern float cmsFront;
extern float cmsLeft;
extern float cmsRight;
extern float cmsRear;
extern int botdrivestate;


extern int epollFd;
extern int updatedisplayFlag;
extern volatile sig_atomic_t terminationRequired;

extern int botdrive_M1_A_GpioFd;
extern int botdrive_M1_B_GpioFd;
extern int botdrive_M2_A_GpioFd;
extern int botdrive_M2_B_GpioFd;

extern int automodestate;

int autoModeduration = 60;
static int UVLED_GpioFd = -1;


void stopBot() {
    GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_Low);
    botdrivestate = 0;
}


void moveBotForward() {
    GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_High);
    GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_High);
    botdrivestate = 1;
}

void moveBotBackwards() {
    GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_High);
    GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_High);
    GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_Low);
    botdrivestate = 2;
}

void turnBotLeft() {

    GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_High);
    GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_High);
    botdrivestate = 3;
   
}

void turnBotRight() {
    GPIO_SetValue(botdrive_M1_A_GpioFd, GPIO_Value_Low);
    GPIO_SetValue(botdrive_M1_B_GpioFd, GPIO_Value_High);
    GPIO_SetValue(botdrive_M2_A_GpioFd, GPIO_Value_High);
    GPIO_SetValue(botdrive_M2_B_GpioFd, GPIO_Value_Low);
    botdrivestate = 4;

}


void checkAndCorrectdistance(float val, int side, int tP) {
    //side 1-Front, 2-Right, 3-Rear, 4-Left
    struct timespec tsWait = { tP, 0 };

    if (side == 1 && val <= criticalDLimit)
    {
        if (cmsLeft > safeDLimit) {
            turnBotLeft();
            nanosleep(&tsWait, NULL);
            stopBot();
        }

        else if (cmsRight > safeDLimit) {
            turnBotRight();
            nanosleep(&tsWait, NULL);
            stopBot();
        }
        
        else {
            moveBotBackwards();
            nanosleep(&tsWait, NULL);
            stopBot();
        }

    }

    else if (side == 2 && val <= criticalDLimit)
    {
        turnBotLeft();
        nanosleep(&tsWait, NULL);
        stopBot();

    }

    else if (side == 3 && val <= criticalDLimit)
    {
        moveBotForward();
        nanosleep(&tsWait, NULL);
        stopBot();

    }

    else if (side == 4 && val <= criticalDLimit)
    {
        turnBotRight();
        nanosleep(&tsWait, NULL);
        stopBot();
     }
    

}



void checkAndMakeCriticalBotAlign() {

    if (cmsFront <= criticalDLimit || 
        cmsLeft <= criticalDLimit || 
        cmsRight <= criticalDLimit ||
        cmsRear <= criticalDLimit)
    {
        stopBot();
        checkAndCorrectdistance(cmsRight,2,3);
        checkAndCorrectdistance(cmsLeft, 4,3);
        checkAndCorrectdistance(cmsFront,1,3);
        checkAndCorrectdistance(cmsRear, 3,3);
    }

    CorrectionInprogressFlag = 0;
}



void checkAndMakeStandardBotAlign() {

    if (cmsFront <= criticalDLimit ||
        cmsLeft <= criticalDLimit ||
        cmsRight <= criticalDLimit ||
        cmsRear <= criticalDLimit)
    {
        stopBot();
        checkAndCorrectdistance(cmsRight, 2, 1);
        checkAndCorrectdistance(cmsLeft, 4, 1);
        checkAndCorrectdistance(cmsFront, 1, 1);
        checkAndCorrectdistance(cmsRear, 3, 1);
    }

    CorrectionInprogressFlag = 0;
}


void checkAndKeepMoving() {

    moveBotForward();
    CorrectionInprogressFlag = 0;

}



static void SanitizeAlgoTimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(sanitizeAlgoPollTimerFd) != 0) {
        terminationRequired = true;
        return;
    }

    if (automodestate == 1)
    {
        UVLED_GpioFd = GPIO_OpenAsOutput(AVNET_MT3620_SK_GPIO29, GPIO_OutputMode_PushPull, GPIO_Value_High);
        if (UVLED_GpioFd < 0)
        {
            Log_Debug("ERROR: Could not open UVLED_GpioFd: %s (%d).\n", strerror(errno), errno);
            return -1;
        }
        automodestate = 2;
    }
    else if (automodestate == 2)
    {

        if (autoModeduration <= 0)
        {
            GPIO_SetValue(UVLED_GpioFd, GPIO_Value_Low);
            stopBot();
            updatedisplayFlag = 1;
            automodestate = 0;
            update_display(0, 0, 0, 0, updatedisplayFlag);
            CloseFdAndPrintError(UVLED_GpioFd, "UVLED_GpioFd");
           // CloseSanitizeAlgoEvent();
        }

        if (CorrectionInprogressFlag == 0) {

            if (cmsFront <= criticalDLimit || cmsLeft <= criticalDLimit || cmsRight <= criticalDLimit )
            {
                CorrectionInprogressFlag = 1;
                checkAndMakeCriticalBotAlign();
                checkAndKeepMoving();
            }
            else if (cmsFront <= safeDLimit || cmsLeft <= safeDLimit || cmsRight <= safeDLimit)
            {
                CorrectionInprogressFlag = 1;
                checkAndMakeStandardBotAlign();
                checkAndKeepMoving();
            }
            else
            {
                CorrectionInprogressFlag = 1;
                checkAndKeepMoving();
            }

        }


        /* if (updatedisplayFlag == 7)
         {
             update_display(cmsFront, cmsLeft, cmsRight, cmsRear, updatedisplayFlag);
         }*/

    }
   
}

static void SanitizeAlgoElapsedTimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(sanitizeAlgoPollElapsedTimerFd) != 0) {
        terminationRequired = true;
        return;
    }

    if (automodestate == 2)
    {

        if (autoModeduration > 0)
        {
            autoModeduration--;
        }


        if (updatedisplayFlag == 7)
        {
            update_display(0, 0, 0, (float)autoModeduration, updatedisplayFlag);
        }


    }


}


int initSanitizeAlgorithm(void) {

    struct timespec sanitizeAlgoCheckPeriod = { SANITIZEALGO_PERIOD_SECONDS, SANITIZEALGO_PERIOD_NANO_SECONDS };

    static EventData sanitizeAlgoTimerEventData = { .eventHandler = &SanitizeAlgoTimerEventHandler };

    sanitizeAlgoPollTimerFd = CreateTimerFdAndAddToEpoll(
        epollFd,
        &sanitizeAlgoCheckPeriod,
        &sanitizeAlgoTimerEventData,
        EPOLLIN);

    if (sanitizeAlgoPollTimerFd < 0)
    {
        return -1;
    }


    struct timespec durationElapsedCheckPeriod = { SANITIZEALGO_PERIOD_ELAPSED_SECONDS, SANITIZEALGO_PERIOD_ELAPSED_NANO_SECONDS };

    static EventData sanitizeAlgoElapsedTimerEventData = { .eventHandler = &SanitizeAlgoElapsedTimerEventHandler };

    sanitizeAlgoPollElapsedTimerFd = CreateTimerFdAndAddToEpoll(
        epollFd,
        &durationElapsedCheckPeriod,
        &sanitizeAlgoElapsedTimerEventData,
        EPOLLIN);

    if (sanitizeAlgoPollElapsedTimerFd < 0)
    {
        return -1;
    }

   

    return 0;
}


void CloseSanitizeAlgoEvent(void) {

    CloseFdAndPrintError(sanitizeAlgoPollTimerFd, "sanitizeAlgoPollTimerFd");
    CloseFdAndPrintError(sanitizeAlgoPollElapsedTimerFd, "sanitizeAlgoPollElapsedTimerFd");
    CloseFdAndPrintError(UVLED_GpioFd, "UVLED_GpioFd");
}