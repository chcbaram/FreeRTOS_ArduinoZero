#include <RTOS.h>



osSemaphoreId sem_serial;



static void threadLoop(void const *argument);
static void threadCount(void const *argument);


uint32_t cnt = 0;


void setup() 
{
  SerialUSB.begin(115200);

  osSemaphoreDef(sem_serial);

  sem_serial = osSemaphoreCreate(osSemaphore(sem_serial), 1);
  

  // define thread
  osThreadDef(threadLoop,  threadLoop,   osPriorityNormal, 0, 1024);
  osThreadDef(threadCount, threadCount,  osPriorityNormal, 0, 1024);

  // create thread
  osThreadCreate(osThread(threadLoop), NULL);
  osThreadCreate(osThread(threadCount), NULL);

  // start kernel
  osKernelStart();

}

void loop() 
{

}


static void threadLoop(void const *argument)
{
  (void) argument;
    
  for(;;)
  {

    if(osSemaphoreWait(sem_serial, 100) == osOK)
    {
      SerialUSB.print("RTOS 1 Cnt : ");
      SerialUSB.println(cnt++);
      osSemaphoreRelease(sem_serial);  
      
      osDelay(500);  
    }
  }
}

static void threadCount(void const *argument)
{
  (void) argument;

  for(;;)
  {
    cnt++;

    if(osSemaphoreWait(sem_serial, 100) == osOK)
    {
      SerialUSB.println("RTOS 2 Running");
      osSemaphoreRelease(sem_serial);     
      osDelay(100);  
    }
  }
}
