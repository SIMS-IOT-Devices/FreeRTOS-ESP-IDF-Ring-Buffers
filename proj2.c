#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"

TaskHandle_t task_send_Handle = NULL;
TaskHandle_t task_receive_Handle = NULL;
RingbufHandle_t ringbuffer;

void task_send(void *arg)
{  
   static char tx_item[] = "data 1";
   xRingbufferSend(ringbuffer, tx_item, sizeof(tx_item), pdMS_TO_TICKS(100));
   printf("Send item: %s \n", tx_item);

   while (1)
   {
      vTaskDelay(pdMS_TO_TICKS(1000));
   }
}

void task_receive(void *arg)
{
   size_t item_size;
   char *item = (char *)xRingbufferReceive(ringbuffer, &item_size, pdMS_TO_TICKS(100));

   vRingbufferReturnItem(ringbuffer, (void *)item);
   printf("Received item: %s \n", item);
   printf("Received item size: %d \n", item_size);
   
   while (1)
   {
      vTaskDelay(pdMS_TO_TICKS(1000));
   }
}

void app_main()
{
   // RingBuffer creation
   ringbuffer = xRingbufferCreate(1028, RINGBUF_TYPE_NOSPLIT);

   // Write data to RingBuffer
   xTaskCreate(task_send, "task_send", 4096, NULL, 10, &task_send_Handle);
   vTaskDelay(pdMS_TO_TICKS(1000));

   // Read data from RingBuffer
   xTaskCreate(task_receive, "task_receive", 4096, NULL, 10, &task_receive_Handle);
   vTaskDelay(pdMS_TO_TICKS(1000));
}
