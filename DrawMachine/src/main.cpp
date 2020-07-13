/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "DigitalIoPin.h"
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include "FreeRTOS.h"
#include "task.h"
#include <atomic>

#include <cstring>
#include <queue.h>
#include "Debug.h"
#include "Parser.h"
#include "MotorUser.h"
#include "user_vcom.h"

Debug *debuger;
Parser *parser;
MotorUser *motorUser;
CommandHandler *handler;

extern "C" {
void RIT_IRQHandler(void) {
	motorUser->HandlerMotor();
}
}

/* Sets up system hardware */
static void prvSetupHardware(void) {
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
}

static void debugTask(void *pvParameters) {
	debuger->Run();
}

static void RunCommands(void *pvParameters) {
	while (1) {
		handler->RunCommand();
	}

}

static void GcodeReceiver(void *pvParameters) {
	// Wait for usb cdc semaphores
	CheckBits();
	int max = 80;
	char buff[80];
	int cur = 0;

	while (1) {
		char str[80];
		// Read usd uart
		int len = USB_receive((uint8_t *) str, 79);
		str[len] = '\0';

		// Echo received character back to usb
		USB_send((uint8_t *) str, len);

		// Go through just received buffer
		for (int i = 0; i < len; ++i) {
			char c = str[i];

			// Add char to buffer
			buff[cur] = c;

			// If buffer goes to max or enter is pressed
			if (cur >= max || c == '\n' || c == '\r') {
				// Add null character to end of buffer
				buff[cur] = '\0';

				// Give input to parser
				parser->ParseBuff(buff);
				char ok[] = "OK\r\n";

				// Send OK back to mDraw
				USB_send((uint8_t *) ok, strlen(ok));

				cur = 0;

			} else {
				cur++;
			}
		}
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* the following is required if runtime statistics are to be collected */
extern "C" {
void vConfigureTimerForRunTimeStats(void) {
	Chip_SCT_Init(LPC_SCTSMALL1);
	LPC_SCTSMALL1->CONFIG = SCT_CONFIG_32BIT_COUNTER;
	LPC_SCTSMALL1->CTRL_U = SCT_CTRL_PRE_L(255) | SCT_CTRL_CLRCTR_L; // set prescaler to 256 (255 + 1), and start timer
}

}

int main(void) {
	prvSetupHardware();

	Chip_RIT_Init(LPC_RITIMER);
	NVIC_SetPriority(RITIMER_IRQn,
			configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

	debuger = new Debug();
	motorUser = new MotorUser(debuger);
	handler = new CommandHandler(debuger, motorUser);
	parser = new Parser(debuger, handler);

	// Init event bits
	InitBist();

	xTaskCreate(debugTask, "Debug printer",
	configMINIMAL_STACK_SIZE + 256, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(GcodeReceiver, "Receive G Code",
	configMINIMAL_STACK_SIZE + 515, NULL, (tskIDLE_PRIORITY + 2UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(RunCommands, "Run commands",
	configMINIMAL_STACK_SIZE + 128, NULL, (tskIDLE_PRIORITY + 2UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(cdc_task, "CDC",
	configMINIMAL_STACK_SIZE + 256, NULL, (tskIDLE_PRIORITY + 2UL),
			(TaskHandle_t *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

