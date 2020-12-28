/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Bluetooth.h"
#include "MeasurementRecord.h"
#include "hci.h"
#include <stdio.h>
#include <string.h>

#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
struct Position
{
  Position(int x, int y) : x(x),y(y){};
  int x;
  int y;
};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
MeasurementRecord measureRecord;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_DAC_Init(void);
/* USER CODE BEGIN PFP */

Position sensorA = {180, 0};
Position sensorB = {0, 180};
Position sensorC = {-180, 0};
Position sensorD = {0, -180};
const long double Ax = static_cast<long double>(sensorA.x) / 1000.0;
const long double Ay = static_cast<long double>(sensorA.y) / 1000.0;
const long double Bx = static_cast<long double>(sensorB.x) / 1000.0;
const long double By = static_cast<long double>(sensorB.y) / 1000.0;
const long double Cx = static_cast<long double>(sensorC.x) / 1000.0;
const long double Cy = static_cast<long double>(sensorC.y) / 1000.0;
const long double Dx = static_cast<long double>(sensorD.x) / 1000.0;
const long double Dy = static_cast<long double>(sensorD.y) / 1000.0;
const long double nominalTargetRadius = By;
const int maxC = 5000;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
Position calculatePosition(const double timeSensorA, const double timeSensorB, const double timeSensorC, const double timeSensorD)
{
  long double tA = timeSensorA / 10000000;
  long double tB = timeSensorB / 10000000;
  long double tC = timeSensorC / 10000000;
  long double tD = timeSensorD / 10000000;

  long double nominator = (2 * (Ax * (By * (tC - tD) + Dy * (tB - tC)) - (By * (tA - tD) - Dy * (tA - tB)) * Cx));

  long double pxk = (Ax * Ax * (By * (tC - tD) + Dy * (tB - tC)) - By * By * Dy * (tA - tC) - By * (Cx * Cx * (tA - tD) - Dy * Dy * (tA -tC)) + Cx * Cx * Dy * (tA - tB)) /
      nominator;

  long double pxc = ((By * (tA - tD) * (tC - tD) + Dy * (tA - tB) * (tB -tC)) * (tC - tA)) /
      nominator;

  long double pyk = (Ax * Ax * Cx * (tB - tD) + Ax * (By * By * (tC - tD) - Cx * Cx * (tB - tD) + Dy * Dy * (tB - tC)) - (By * By * (tA - tD) - Dy * Dy * (tA - tB)) * Cx) /
      nominator;
  long double pyc = (Ax * (tB - tC) * (tC - tD) + Cx * (tA * tA - tA * (tB + tD) + tB * tD)) * (tD - tB) /
      nominator;

  long double t0c = (Ax * (By * (tC * tC - tD * tD) + Dy * (tB * tB - tC * tC)) - (By * (tA * tA - tD * tD) - Dy * (tA * tA - tB * tB)) * Cx) /
      nominator;

  long double t0k = ((Ax - Cx) * (Ax * Cx - By * Dy) * (By - Dy)) /
      nominator;

  long double finalC = 0;
  long double error = 100000;

  for(int index = 1; index < maxC; ++index)
  {
    long double c = static_cast<long double>(index);
    long double currentError = std::abs((((pxk + pxc * c * c) - Ax) * ((pxk + pxc * c * c) - Ax) + (pyk + pyc * c * c) * (pyk + pyc * c * c)) - ((c * (tA - (t0c + t0k / (c * c)))) * (c * (tA - (t0c + t0k / (c * c))))));
    if(currentError < error)
    {
      error = currentError;
      finalC = c;
    }
  }

  long double c = finalC / 2;

  long double Px = (pxk + pxc * c * c) * 1000.0;
  long double Py = (pyk + pyc * c * c) * 1000.0;

  return Position(static_cast<int>(Px), static_cast<int>(Py));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
  Bluetooth bluetooth;
  bluetooth.initialize();


  HAL_TIM_Base_Start(&htim1);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 200);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  if(measureRecord.newRecordCapured())
  {
    Position strikePosition = calculatePosition(measureRecord.timeSensorA, measureRecord.timeSensorB, measureRecord.timeSensorC, measureRecord.timeSensorD);
    measureRecord.reset();
    bluetooth.updatePositionCharacteristics(strikePosition.x, strikePosition.y);
  }
  if(measureRecord.measurementOngoing && htim1.Instance->CNT > measureRecord.maxMeasureTime)
  { // Not all sensors were triggered, therefore reset and wait for another impact
    measureRecord.reset();
  }
    hci_user_evt_proc();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 17;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xFFFF;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1 PC2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(!measureRecord.measurementOngoing)
  {
    measureRecord.measurementOngoing = true;
    htim1.Instance->CNT = 0;
  }
  switch(GPIO_Pin)
  {
  case GPIO_PIN_1:
    if(!measureRecord.registeredSensorA)
    {
      measureRecord.timeSensorA = htim1.Instance->CNT;
      measureRecord.registeredSensorA = true;
    }
    break;
  case GPIO_PIN_2:
    if(!measureRecord.registeredSensorB)
    {
      measureRecord.timeSensorB = htim1.Instance->CNT;
      measureRecord.registeredSensorB = true;
    }
    break;
  case GPIO_PIN_4:
    if(!measureRecord.registeredSensorC)
    {
      measureRecord.timeSensorC = htim1.Instance->CNT;
      measureRecord.registeredSensorC = true;
    }
    break;
  case GPIO_PIN_5:
    if(!measureRecord.registeredSensorD)
    {
      measureRecord.timeSensorD = htim1.Instance->CNT;
      measureRecord.registeredSensorD = true;
    }
    break;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
