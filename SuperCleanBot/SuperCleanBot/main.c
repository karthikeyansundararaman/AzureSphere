#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "epoll_timerfd_utilities.h"
#include "i2c.h"
#include "hw/avnet_mt3620_sk.h"
#include "inputsense.h"
#include "botdrive.h"
#include "ultrasonicsensor.h"
#include "lightsensor.h"
#include "sanitizeAlgo.h"

#include <applibs/log.h>
#include <applibs/i2c.h>
#include <applibs/gpio.h>



// Support functions.
static void TerminationHandler(int signalNumber);
static int InitPeripheralsAndHandlers(void);
static void ClosePeripheralsAndHandlers(void);

// File descriptors - initialized to invalid value
int epollFd = -1;

int botdrivestate = 0;
int updatedisplayFlag = 1;

int automodestate = 0;

int botdrive_M1_A_GpioFd = -1;
int botdrive_M1_B_GpioFd = -1;
int botdrive_M2_A_GpioFd = -1;
int botdrive_M2_B_GpioFd = -1;

// Termination state
volatile sig_atomic_t terminationRequired = false;


/// <summary>
///     Signal handler for termination requests. This handler must be async-signal-safe.
/// </summary>
static void TerminationHandler(int signalNumber)
{
    // Don't use Log_Debug here, as it is not guaranteed to be async-signal-safe.
    terminationRequired = true;
}

static int InitPeripheralsAndHandlers(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = TerminationHandler;
    sigaction(SIGTERM, &action, NULL);

    epollFd = CreateEpollFd();
    if (epollFd < 0) {
        return -1;
    }

    if (initI2c() == -1) {
        return -1;
    }

    if (initInputSense() == -1) {
        return -1;
    }

    if (initBotDrive() == -1) {
        return -1;
    }

    if (initUltrasonicSensor() == -1) {
        return -1;
    }

    if ( initLightSensor() == -1) {
        return -1;
    }

    if (initSanitizeAlgorithm() == -1) {
        return -1;
    }

   return 0;
}

/// <summary>
///     Close peripherals and handlers.
/// </summary>
static void ClosePeripheralsAndHandlers(void)
{
    Log_Debug("Closing file descriptors.\n");

    closeI2c();
    closeInputSense();
    closeBotDrive();
    closeUltrasonicSensor();
    closeLightSensor();

    CloseSanitizeAlgoEvent();

    CloseFdAndPrintError(epollFd, "Epoll");

}

/// <summary>
///     Main entry point for this application.
/// </summary>
int main(int argc, char* argv[])
{

    Log_Debug("Super-Clean-Bot Status: Getting Ready... \n");
    if (InitPeripheralsAndHandlers() != 0) {
        Log_Debug("Super-Clean-Bot Status: Initialization Failed! \n");
        terminationRequired = true;
    }
    Log_Debug("Super-Clean-Bot Status: Ready for Cleaning \n");

    // Use epoll to wait for events and trigger handlers, until an error or SIGTERM happens
    while (!terminationRequired) {
        if (WaitForEventAndCallHandler(epollFd) != 0) {
            terminationRequired = true;
        }

    }

    ClosePeripheralsAndHandlers();
    Log_Debug("Application exiting.\n");
    return 0;
}
