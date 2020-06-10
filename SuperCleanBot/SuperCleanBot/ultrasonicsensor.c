#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

#include <applibs/gpio.h>
#include <applibs/log.h>


#include "ultrasonicsensor.h"
#include "hw/avnet_mt3620_sk.h"
#include "oled.h"


#define USENSOR_PERIOD_SECONDS 0
#define USENSOR_PERIOD_NANO_SECONDS 500000000

static int ultrasonicSensorPollTimerFd = -1;

float cmsFront;
float cmsLeft;
float cmsRight;
float cmsRear;


extern int epollFd;
extern int updatedisplayFlag;
extern volatile sig_atomic_t terminationRequired;


void calcTimeElapsed(const struct timespec* endtime, const struct timespec* starttime, struct timespec* difftime)
{
    difftime->tv_sec = endtime->tv_sec - starttime->tv_sec;
    difftime->tv_nsec = endtime->tv_nsec - starttime->tv_nsec;
    if (difftime->tv_nsec < 0)
    {
        difftime->tv_sec -= 1;
        difftime->tv_nsec += 1e9;
    }
}


static float getultrasonicsensoreading(GPIO_Id trig, GPIO_Id echo)
{
    struct timespec tsTrigRise = { 0,0 };
    struct timespec tsTrigFall = { 0,0 };

    struct timespec tsWait = { 0, 10000 }; // 10micro sec timer

    int trigfd = GPIO_OpenAsOutput(trig, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (trigfd < 0) {
        Log_Debug("ERROR: Could not open Ultrasonic Output GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }
    nanosleep(&tsWait, NULL);
    GPIO_SetValue(trigfd, GPIO_Value_Low);

    close(trigfd);
   

    int echofd = GPIO_OpenAsInput(echo);
    if (echofd < 0) {
        Log_Debug("ERROR: Could not open Ultrasonic Input GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }

    bool istriggered = false;
    GPIO_Value_Type echoVal;

    while (true)
    {

        GPIO_GetValue(echofd, &echoVal);
        if (echoVal == GPIO_Value_High)
        {
            if (!istriggered)
            {
                clock_gettime(CLOCK_MONOTONIC, &tsTrigRise);
                istriggered = true;
               /* Log_Debug("StartTime(sec) %d \n", tsTrigRise.tv_sec);
                Log_Debug("StartTime(nsec) %d \n", tsTrigRise.tv_nsec);*/
            }
        }
        else
        {
            if (istriggered)
            {
                clock_gettime(CLOCK_MONOTONIC, &tsTrigFall);
               /* Log_Debug("EndTime(sec) %d \n", tsTrigFall.tv_sec);
                Log_Debug("EndTime(nsec) %d \n", tsTrigFall.tv_nsec);*/
                break;
            }
        }

    }
    close(echofd);

    if (tsTrigFall.tv_sec == 0 && tsTrigFall.tv_nsec == 0)
    {
        return 400;
    }

    struct timespec echoDuration = { 0,0 };
    calcTimeElapsed(&tsTrigFall, &tsTrigRise, &echoDuration);

     return (echoDuration.tv_nsec * 0.000017);
}



static void UltrasonicTimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(ultrasonicSensorPollTimerFd) != 0) {
        terminationRequired = true;
        return;
    }

    cmsFront = getultrasonicsensoreading(AVNET_MT3620_SK_GPIO1, AVNET_MT3620_SK_GPIO16);
    Log_Debug("Front - Approx %.1f cm\n", cmsFront);

    cmsLeft = getultrasonicsensoreading(AVNET_MT3620_SK_GPIO0, AVNET_MT3620_SK_GPIO2);
    Log_Debug("Left - Approx %.1f cm\n", cmsLeft);

    cmsRight = getultrasonicsensoreading(AVNET_MT3620_SK_GPIO28, AVNET_MT3620_SK_GPIO26);
    Log_Debug("Right - Approx %.1f cm\n", cmsRight);

    cmsRear = getultrasonicsensoreading(AVNET_MT3620_SK_GPIO35, AVNET_MT3620_SK_GPIO17);
    Log_Debug("Rear - Approx %.1f cm\n", cmsRear);

    if (updatedisplayFlag == 2)
    {
        update_display(cmsFront, cmsLeft, cmsRight, cmsRear, updatedisplayFlag);
    }

    //oled_draw_logo();

    //Log_Debug("Appox %.1f cm\n");
}


int initUltrasonicSensor(void) {

    struct timespec sensorCheckPeriod = { USENSOR_PERIOD_SECONDS, USENSOR_PERIOD_NANO_SECONDS };
    
    static EventData UltrasonicTimerEventData = { .eventHandler = &UltrasonicTimerEventHandler };

    ultrasonicSensorPollTimerFd = CreateTimerFdAndAddToEpoll(
        epollFd, 
        &sensorCheckPeriod, 
        &UltrasonicTimerEventData, 
        EPOLLIN);
    
    if (ultrasonicSensorPollTimerFd < 0)
    {
        return -1;
    }

    return 0;
}


void closeUltrasonicSensor(void) {

  CloseFdAndPrintError(ultrasonicSensorPollTimerFd, "ultrasonicSensorPollTimerFd");
}