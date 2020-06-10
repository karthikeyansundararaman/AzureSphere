#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

#include <applibs/gpio.h>
#include <applibs/log.h>

#include "inputsense.h"
#include "hw/avnet_mt3620_sk.h"
#include "oled.h"


#define INPUTSENSE_BTN_PERIOD_SECONDS 0
#define INPUTSENSE_BTN_PERIOD_NANO_SECONDS 500000000

static int inputsensebtnAPollTimerFd = -1;
static int inputsensebtnBPollTimerFd = -1;
static int inputsensebtn_A_GpioFd = -1;
static int inputsensebtn_B_GpioFd = -1;

//static GPIO_Value_Type inputsensebtn_A_State = GPIO_Value_High;
//static GPIO_Value_Type inputsensebtn_B_State = GPIO_Value_High;

extern int automodestate;

extern int epollFd;
extern int botdrivestate;
extern int updatedisplayFlag;
extern volatile sig_atomic_t terminationRequired;


static void inputsensebtnATimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(inputsensebtnAPollTimerFd) != 0)
    {
        terminationRequired = true;
        return;
    }

    if (automodestate == 0) {

    

    GPIO_Value_Type newinputsensebtn_A_State;
    GPIO_Value_Type newinputsensebtn_B_State;

    int btn_A_result = GPIO_GetValue(inputsensebtn_A_GpioFd, &newinputsensebtn_A_State);
    if (btn_A_result != 0)
    {
        Log_Debug("ERROR: Could not read button_A GPIO: %s (%d).\n", strerror(errno), errno);
        terminationRequired = true;
        return;
    }

    int btn_B_result = GPIO_GetValue(inputsensebtn_B_GpioFd, &newinputsensebtn_B_State);
    if (btn_B_result != 0)
    {
        Log_Debug("ERROR: Could not read button_B GPIO: %s (%d).\n", strerror(errno), errno);
        terminationRequired = true;
        return;
    }

   /* if (newinputsensebtn_A_State != inputsensebtn_A_State) {*/

        if (newinputsensebtn_A_State == GPIO_Value_Low && newinputsensebtn_B_State == GPIO_Value_High)
        {

            switch (botdrivestate)
            {
            case 0:
                botdrivestate = 1;
                Log_Debug("Command: Forward Triggered\n");
                break;

            case 1:
                botdrivestate = 2;
                Log_Debug("Command: Reverse Triggered\n");
                break;

            case 2:
                botdrivestate = 3;
                Log_Debug("Command: Left Triggered\n");
                break;

            case 3:
                botdrivestate = 4;
                Log_Debug("Command: Right Triggered\n");
                break;

            case 4:
                botdrivestate = 0;
                Log_Debug("Command: Halt/Stop Triggered\n");
                break;

            default:
                botdrivestate = 0;
                break;
            }
        }
        else  if (newinputsensebtn_A_State == GPIO_Value_Low && newinputsensebtn_B_State == GPIO_Value_Low)
        {
            automodestate = 1;
            updatedisplayFlag = 7;
           
        }

        else
        {
            //Do Nothing
        }

    /*}

    inputsensebtn_A_State = newinputsensebtn_A_State;
    inputsensebtn_B_State = newinputsensebtn_B_State;*/
    
    }

}

static void inputsensebtnBTimerEventHandler(EventData* eventData)
{
    if (ConsumeTimerFdEvent(inputsensebtnBPollTimerFd) != 0)
    {
        terminationRequired = true;
        return;
    }

    if (automodestate == 0)
    {
        GPIO_Value_Type newinputsensebtn_A_State;
        GPIO_Value_Type newinputsensebtn_B_State;

        int btn_A_result = GPIO_GetValue(inputsensebtn_A_GpioFd, &newinputsensebtn_A_State);
        if (btn_A_result != 0)
        {
            Log_Debug("ERROR: Could not read button_A GPIO: %s (%d).\n", strerror(errno), errno);
            terminationRequired = true;
            return;
        }

        int btn_B_result = GPIO_GetValue(inputsensebtn_B_GpioFd, &newinputsensebtn_B_State);
        if (btn_B_result != 0)
        {
            Log_Debug("ERROR: Could not read button_B GPIO: %s (%d).\n", strerror(errno), errno);
            terminationRequired = true;
            return;
        }

        /*if (newinputsensebtn_B_State != inputsensebtn_B_State) {*/

        if (newinputsensebtn_A_State == GPIO_Value_High && newinputsensebtn_B_State == GPIO_Value_Low)
        {

            switch (updatedisplayFlag)
            {
            case 1:
                updatedisplayFlag = 2;
                Log_Debug("Command: Display Page2 Triggered\n");
                break;

            case 2:
                updatedisplayFlag = 3;
                Log_Debug("Command: Display Page3 Triggered\n");
                break;

            case 3:
                updatedisplayFlag = 4;
                Log_Debug("Command: Display Page4 Triggered\n");
                break;

            case 4:
                updatedisplayFlag = 5;
                Log_Debug("Command: Display Page5 Triggered\n");
                break;

            case 5:
                updatedisplayFlag = 6;
                Log_Debug("Command: Display Page6 Triggered\n");
                break;

            case 6:
                updatedisplayFlag = 1;
                Log_Debug("Command: Display Page1 Triggered\n");
                if (updatedisplayFlag == 1)
                {
                    update_display(0, 0, 0, 0, updatedisplayFlag);
                }
                break;

            default:
                updatedisplayFlag = 1;
                break;
            }
        }
        else  if (newinputsensebtn_A_State == GPIO_Value_Low && newinputsensebtn_B_State == GPIO_Value_Low)
        {
            automodestate = 1;
            updatedisplayFlag = 7;
        }

        else
        {
            //Do Nothing
        }

        /*  }

          inputsensebtn_B_State = newinputsensebtn_B_State;
          inputsensebtn_A_State = newinputsensebtn_B_State;*/
    }
}


int initInputSense(void) {

    inputsensebtn_A_GpioFd = GPIO_OpenAsInput(AVNET_MT3620_SK_USER_BUTTON_A);
    inputsensebtn_B_GpioFd = GPIO_OpenAsInput(AVNET_MT3620_SK_USER_BUTTON_B);

	if (inputsensebtn_A_GpioFd < 0) {
		Log_Debug("ERROR: Could not open inputsense - button A GPIO: %s (%d).\n", strerror(errno), errno);
		return -1;
	}

    if (inputsensebtn_B_GpioFd < 0) {
        Log_Debug("ERROR: Could not open inputsense - button B GPIO: %s (%d).\n", strerror(errno), errno);
        return -1;
    }

	struct timespec inputsensebtnCheckPeriod = { INPUTSENSE_BTN_PERIOD_SECONDS, INPUTSENSE_BTN_PERIOD_NANO_SECONDS };

    static EventData inputsensebtnAEventData = { .eventHandler = &inputsensebtnATimerEventHandler };

	inputsensebtnAPollTimerFd = CreateTimerFdAndAddToEpoll(
        epollFd, 
        &inputsensebtnCheckPeriod,
        &inputsensebtnAEventData,
        EPOLLIN);
	
    if (inputsensebtnAPollTimerFd < 0)
	{
		return -1;
	}

    static EventData inputsensebtnBEventData = { .eventHandler = &inputsensebtnBTimerEventHandler };

    inputsensebtnBPollTimerFd = CreateTimerFdAndAddToEpoll(
        epollFd,
        &inputsensebtnCheckPeriod,
        &inputsensebtnBEventData,
        EPOLLIN);

    if (inputsensebtnBPollTimerFd < 0)
    {
        return -1;
    }

    if (updatedisplayFlag == 1)
    {
        update_display(0, 0, 0, 0, updatedisplayFlag);
    }

	return 0;
}


void closeInputSense(void) {

	CloseFdAndPrintError(inputsensebtn_A_GpioFd, "inputsense_button_A_Fd");
    CloseFdAndPrintError(inputsensebtn_B_GpioFd, "inputsense_button_B_Fd");
	CloseFdAndPrintError(inputsensebtnAPollTimerFd, "inputsense_buttonA_Timer_Fd");
    CloseFdAndPrintError(inputsensebtnBPollTimerFd, "inputsense_buttonB_Timer_Fd");

}