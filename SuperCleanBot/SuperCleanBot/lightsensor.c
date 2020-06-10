#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <applibs/adc.h>
#include <applibs/gpio.h>
#include <applibs/log.h>


#include "applibs_versions.h"
#include "oled.h"
#include "lightsensor.h"
#include "hw/avnet_mt3620_sk.h"



#define LSENSOR_PERIOD_SECONDS 1
#define LSENSOR_PERIOD_NANO_SECONDS 0

static int lightSensorPollTimerFd = -1;
static int lightSensor_ADCFd = -1;

float light_sensor; 

//static int trigfd = -1;
//static int echofd = -1;

extern int epollFd;
extern int updatedisplayFlag;
extern volatile sig_atomic_t terminationRequired;



static void lightsensorTimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(lightSensorPollTimerFd) != 0) {
        terminationRequired = true;
        return;
    }

    uint32_t value;
    int result = ADC_Poll(lightSensor_ADCFd, AVNET_MT3620_SK_ADC_CONTROLLER0, &value);
    if (result == -1) {
        Log_Debug("ADC_Poll failed with error: %s (%d)\n", strerror(errno), errno);
        return;
    }

    // get voltage (2.5*adc_reading/4096)
    // divide by 3650 (3.65 kohm) to get current (A)
    // multiply by 1000000 to get uA
    // divide by 0.1428 to get Lux (based on fluorescent light Fig. 1 datasheet)
    // divide by 0.5 to get Lux (based on incandescent light Fig. 1 datasheet)
    // We can simplify the factors, but for demostration purpose it's OK
    light_sensor = ((float)value * 2.5 / 4095) * 1000000 / (3650 * 0.1428);

    Log_Debug("Ambient Light value is %.3f lux\n", light_sensor);

    if (updatedisplayFlag == 6)
    {
        update_display(0, 0, 0, light_sensor, updatedisplayFlag);
    }
    

    //Log_Debug("Appox %.1f cm\n");
}


int initLightSensor(void) {

    lightSensor_ADCFd = ADC_Open(AVNET_MT3620_SK_ADC_CONTROLLER0);
    if (lightSensor_ADCFd == -1) {
        Log_Debug("ADC_Open failed with error: %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    struct timespec sensorCheckPeriod = { LSENSOR_PERIOD_SECONDS, LSENSOR_PERIOD_NANO_SECONDS };

    static EventData LightsensorTimerEventData = { .eventHandler = &lightsensorTimerEventHandler };

    lightSensorPollTimerFd = CreateTimerFdAndAddToEpoll(
        epollFd,
        &sensorCheckPeriod,
        &LightsensorTimerEventData,
        EPOLLIN);

    if (lightSensorPollTimerFd < 0)
    {
        return -1;
    }

    return 0;
}



void closeLightSensor(void) {

    /* CloseFdAndPrintError(trigfd, "trigfd");
     CloseFdAndPrintError(echofd, "echofd");*/
    CloseFdAndPrintError(lightSensorPollTimerFd, "lightSensorPollTimerFd");
}