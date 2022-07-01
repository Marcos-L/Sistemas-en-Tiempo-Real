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
#include "variables.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
float IR;
float LR;
float NDVI;
int Hum;
int Lum;

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
/* Definitions for Read_Sensor */
osThreadId_t Read_SensorHandle;
const osThreadAttr_t Read_Sensor_attributes = {
  .name = "Read_Sensor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for NDVIRead */
osThreadId_t NDVIReadHandle;
const osThreadAttr_t NDVIRead_attributes = {
  .name = "NDVIRead",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for Feedback */
osThreadId_t FeedbackHandle;
const osThreadAttr_t Feedback_attributes = {
  .name = "Feedback",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Debounce */
osThreadId_t DebounceHandle;
const osThreadAttr_t Debounce_attributes = {
  .name = "Debounce",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartRead_Sensor(void *argument);
void StartNDVIRead(void *argument);
void StartFeedback(void *argument);
void StartDebounce(void *argument);

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

  /* creation of Read_Sensor */
  Read_SensorHandle = osThreadNew(StartRead_Sensor, NULL, &Read_Sensor_attributes);

  /* creation of NDVIRead */
  NDVIReadHandle = osThreadNew(StartNDVIRead, NULL, &NDVIRead_attributes);

  /* creation of Feedback */
  FeedbackHandle = osThreadNew(StartFeedback, NULL, &Feedback_attributes);

  /* creation of Debounce */
  DebounceHandle = osThreadNew(StartDebounce, NULL, &Debounce_attributes);

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

/* USER CODE BEGIN Header_StartRead_Sensor */
/**
* @brief Function implementing the Read_Sensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRead_Sensor */
void StartRead_Sensor(void *argument)
{
  /* USER CODE BEGIN StartRead_Sensor */
  /* Infinite loop */
	for(;;){
	  vTaskSuspend(NULL);
	  char n = 3;
	  char Msg[n];
	  int s = 0;
	  HAL_UART_Receive(&huart6, Msg, n, 5000);

	  for(int i = 0;i<n-1;i++){
		  s += Msg[i];
	  }

	  if(s==Msg[n-1]){
		  Hum = Msg[0];
		  Lum = Msg[1];
	  }
	}
  /* USER CODE END StartRead_Sensor */
}

/* USER CODE BEGIN Header_StartNDVIRead */
/**
* @brief Function implementing the NDVIRead thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartNDVIRead */
void StartNDVIRead(void *argument)
{
  /* USER CODE BEGIN StartNDVIRead */
  /* Infinite loop */
	for(;;){
	  vTaskSuspend(NULL);
	  NDVI = (IR-LR)/(IR+LR);
	  NDVI *= 100;
	  int NDVI_int = (int)NDVI;

	  switch (NDVI_int){
		  case -100 ... 0:
			  //La planta esta muetra
			  break;
		  case 1 ... 33:
			  //La planta necesita Agua o Luz o Ambas
			  //Activo la lectura de los sensores
		  	  vTaskResume(Read_SensorHandle);
			  break;
		  case 34 ... 66:
			  //La planta necesita Agua o Luz o Ambas
			  //Activo la lectura de los sensores
		  	  vTaskResume(Read_SensorHandle);
			  break;
		  case 67 ... 100:
			  //La planta esta perfecta
			  break;
		  default:
			  break;
	  }
	  vTaskResume(FeedbackHandle);
	}
  /* USER CODE END StartNDVIRead */
}

/* USER CODE BEGIN Header_StartFeedback */
/**
* @brief Function implementing the Feedback thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartFeedback */
void StartFeedback(void *argument)
{
  /* USER CODE BEGIN StartFeedback */
  /* Infinite loop */
  for(;;)
  {
	vTaskSuspend(NULL);
	int NDVI_int = (int)NDVI;
	switch (NDVI_int){
	  case -100 ... 0:
		  //La planta esta muetra
	  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		  break;
	  case 1 ... 33:
		  //La planta necesita Agua o Luz o Ambas
		  //Activo la lectura de los sensores
	  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		  break;
	  case 34 ... 66:
		  //La planta necesita Agua o Luz o Ambas
		  //Activo la lectura de los sensores
	  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  break;
	  case 67 ... 100:
		  //La planta esta perfecta
	  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		  break;
	  default:
		  break;
	}
  }
  /* USER CODE END StartFeedback */
}

/* USER CODE BEGIN Header_StartDebounce */
/**
* @brief Function implementing the Debounce thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebounce */
void StartDebounce(void *argument)
{
  /* USER CODE BEGIN StartDebounce */
  /* Infinite loop */
  IR = 3.4;
  LR = 6.2;
  for(;;)
  {
	  vTaskSuspend(NULL);
	  // Inhabilita IRQ
	  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	  osDelay(1);
	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_SET)
	  {
		 vTaskResume(NDVIReadHandle);
	  }
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  }
  /* USER CODE END StartDebounce */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == 8192){
		xTaskResumeFromISR(DebounceHandle);
		}
	else{
		__NOP();
	}
}
/* USER CODE END Application */
