#include <mbed.h>
#include <rtos.h>

typedef struct {
	int      last_value;  
	int      current_value;
} message_t;


osPoolDef(mpool, 16, message_t);
osPoolId  mpool;

osMessageQDef(queue, 16, message_t);
osMessageQId  queue;

#define USB_SERIAL_BAUDRATE 9600
Serial serialMonitor(USBTX, USBRX, USB_SERIAL_BAUDRATE);

DigitalOut g_LED1(LED1);
osThreadId sigHandlerThread, mainThread;

static void ThreadBody(const void *) 
{
	sigHandlerThread = osThreadGetId();
	mpool = osPoolCreate(osPool(mpool));
	int count = 0;
	for (;;)
	{
		count++;
		osSignalWait(0x10, osWaitForever);
		serialMonitor.printf("Signal Received...\r\n");
		g_LED1 = !g_LED1;
		message_t *message = (message_t*)osPoolAlloc(mpool);
		message->current_value = count;
		osMessagePut(queue, (uint32_t)message, osWaitForever);
		
		
	}
}

int main()
{
	mainThread = osThreadGetId();
	Thread thread(ThreadBody);
	queue = osMessageCreate(osMessageQ(queue), NULL);
	
	for (;;)
	{
		Thread::wait(300);
		osSignalSet(sigHandlerThread, 0x10);
		
		osEvent evt = osMessageGet(queue, osWaitForever);
		if (evt.status == osEventMessage) {
			message_t *message = (message_t*)evt.value.p;
			serialMonitor.printf("thread message is: %d\r\n", message->current_value);
			osPoolFree(mpool, message);
		}
	}
}
