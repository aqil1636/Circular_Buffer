#include "stm32f10x.h"
#include "cmsis_os.h"
#include "uart.h"

void Producer_t (void const *argument);
void Consumer_t (void const *argument);
osThreadDef(Producer_t, osPriorityNormal, 1, 0);
osThreadDef(Consumer_t, osPriorityNormal, 1, 0);

osThreadId T_Pro;
osThreadId T_Con;
	
osMutexId x_mutex;
osMutexDef(x_mutex);
osSemaphoreId item_semaphore;                         // Semaphore ID
osSemaphoreDef(item_semaphore);                       // Semaphore definition
osSemaphoreId space_semaphore;                         // Semaphore ID
osSemaphoreDef(space_semaphore);                       // Semaphore definition

long int p=0;
long int c=0;

const unsigned int SIZE = 4;
unsigned char buffer[SIZE];
unsigned int ins_Index = 0;
unsigned int rem_Index = 0;

void put(unsigned char an_item){
	osSemaphoreWait(space_semaphore, osWaitForever);
	osMutexWait(x_mutex, osWaitForever);
	buffer[ins_Index] = an_item;
	ins_Index = (ins_Index + 1) % SIZE;
	osMutexRelease(x_mutex);
	osSemaphoreRelease(item_semaphore);
}

unsigned char get(){
	unsigned int out_data = 0xff;
	osSemaphoreWait(item_semaphore, osWaitForever);
	osMutexWait(x_mutex, osWaitForever);
	out_data = buffer[rem_Index];
	rem_Index = (rem_Index + 1) % SIZE;
	osMutexRelease(x_mutex);
	osSemaphoreRelease(space_semaphore);
	return out_data;
}

int loopcount = 10;

void Producer_t (void const *argument) 
{
	unsigned char item = 0x30;
	for(; p<loopcount; p++){
		put(item++);
	}
}

void Consumer_t (void const *argument) 
{
	unsigned int data = 0x00;
	for(; c<loopcount; c++){
		data = get();
		SendChar(data);
	}
}

int main (void) 
{
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	USART1_Init();
	item_semaphore = osSemaphoreCreate(osSemaphore(item_semaphore), 0);
	space_semaphore = osSemaphoreCreate(osSemaphore(space_semaphore), N);
	x_mutex = osMutexCreate(osMutex(x_mutex));	
	
	T_Pro = osThreadCreate(osThread(Producer_t), NULL);
	T_Con = osThreadCreate(osThread(Consumer_t), NULL);
	osKernelStart ();                         // start thread execution 
}