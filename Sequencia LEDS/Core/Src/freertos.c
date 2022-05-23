/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for debounce */
osThreadId_t debounceHandle;
const osThreadAttr_t debounce_attributes = {
  .name = "debounce",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Seq1 */
osThreadId_t Seq1Handle;
const osThreadAttr_t Seq1_attributes = {
  .name = "Seq1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Seq2 */
osThreadId_t Seq2Handle;
const osThreadAttr_t Seq2_attributes = {
  .name = "Seq2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Seq3 */
osThreadId_t Seq3Handle;
const osThreadAttr_t Seq3_attributes = {
  .name = "Seq3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartDebounce(void *argument);
void StartSeq1(void *argument);
void StartSeq2(void *argument);
void StartSeq3(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of debounce */
  debounceHandle = osThreadNew(StartDebounce, NULL, &debounce_attributes);

  /* creation of Seq1 */
  Seq1Handle = osThreadNew(StartSeq1, NULL, &Seq1_attributes);

  /* creation of Seq2 */
  Seq2Handle = osThreadNew(StartSeq2, NULL, &Seq2_attributes);

  /* creation of Seq3 */
  Seq3Handle = osThreadNew(StartSeq3, NULL, &Seq3_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartDebounce */
/**
* @brief Function implementing the debounce thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebounce */
void StartDebounce(void *argument)
{
  /* USER CODE BEGIN StartDebounce */
  /* Infinite loop */
  int secuencia = 0;
  int secuencias = 3;
  vTaskResume(Seq1Handle);
  for(;;)
  {
	  vTaskSuspend(NULL);
	  // Inhabilita IRQ
	  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	  osDelay(1);
	  if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) ==
			  GPIO_PIN_RESET)
	  {
		// Cambio de secuencia.
		  vTaskSuspend(Seq1Handle);
		  vTaskSuspend(Seq2Handle);
		  vTaskSuspend(Seq3Handle);
		  secuencia = (secuencia + 1) % secuencias;
		  switch (secuencia)
		  {
		  case 0:
			  vTaskResume(Seq1Handle);
			  break;
		  case 1:
			  vTaskResume(Seq2Handle);
			  break;
		  case 2:
		  		vTaskResume(Seq3Handle);
		  	  break;
		  default:
			  break;
		  };
	  }
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  }
  /* USER CODE END StartDebounce */
}

/* USER CODE BEGIN Header_StartSeq1 */
/**
* @brief Function implementing the Seq1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSeq1 */
void StartSeq1(void *argument)
{
  /* USER CODE BEGIN StartSeq1 */
  /* Infinite loop */
	vTaskSuspend(NULL);
	int estado = 0;
	  int r = 20;
	  int g = 100;
	  int b = 0;
	  while (1)
	  {
		  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, r);//R
		  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, g);//G
		  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, b);//B
		  if (r>=100 || b>=100){
			  estado++;
		  }
		  switch(estado){
		  case 0:
			  r+=20;
			  g-=20;
			  b=0;
			  break;
		  case 1:
			  r-=20;
			  g=0;
			  b+=20;
			  break;
		  case 2:
			  r=20;
			  g=100;
			  b=0;
			  estado=0;
			  break;
		  }
		  osDelay(20);
	  }
  /* USER CODE END StartSeq1 */
}

/* USER CODE BEGIN Header_StartSeq2 */
/**
* @brief Function implementing the Seq2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSeq2 */
void StartSeq2(void *argument)
{
  /* USER CODE BEGIN StartSeq2 */
  /* Infinite loop */
	vTaskSuspend(NULL);
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartSeq2 */
}

/* USER CODE BEGIN Header_StartSeq3 */
/**
* @brief Function implementing the Seq3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSeq3 */
void StartSeq3(void *argument)
{
  /* USER CODE BEGIN StartSeq3 */
  /* Infinite loop */
	vTaskSuspend(NULL);
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartSeq3 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_13)
		xTaskResumeFromISR(debounceHandle);
}
/* USER CODE END Application */

