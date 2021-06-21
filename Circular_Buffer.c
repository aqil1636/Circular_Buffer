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