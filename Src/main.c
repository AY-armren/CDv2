/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define X 1
#define Y 0
#define pos 0
#define neg 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart4;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM15_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t run(uint32_t time_acs_ms, uint16_t velocity /*imp/sec*/, TIM_TypeDef * TimX, TIM_HandleTypeDef *htim, uint32_t Channel1, uint32_t Channel2, uint32_t coordinate /*add*/, GPIO_TypeDef * SwitchPort, uint16_t SwitchPin);
uint8_t direction(uint8_t axis /*0-x 1-y*/, uint8_t dir);
uint8_t scan(uint32_t width, uint32_t height, uint8_t N);
uint8_t homing(TIM_TypeDef * TimX, TIM_HandleTypeDef *htim, uint32_t Channel1, uint32_t Channel2, GPIO_TypeDef * SwitchPort, uint16_t SwitchPin);
cmd_uart RS485_TRANSMITTER ={0};
uint8_t rx_buffer;
uint16_t crc;
typedef struct{
	uint16_t velocity;
	uint16_t time_acs;
	uint16_t coordinate;
	uint8_t axis;
	uint8_t dir;
	uint16_t width;
	uint16_t height;
	uint8_t step_number;
	uint16_t status;
}station;
uint8_t proceed(station * structure, cmd_uart * RS485_structure);
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
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_TIM1_Init();
  MX_TIM15_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  TIM15->BDTR |= TIM_BDTR_MOE;
  HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
  HAL_UART_Receive_IT(&huart4, &rx_buffer, 1);
  //homing(TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_10);
  direction(Y, pos);
  //run(100, 5000, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, 8000, GPIOA, GPIO_PIN_10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	/*direction(X, pos);
	run(200, 3000, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, 8000);
	//run(time_acs_ms, velocity, TimX, htim, Channel1, Channel2, coordinate)

	direction(X, neg);
	run(200, 3000, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, 8000);
	scan(8000, 700, 5);*/
	/*if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET){
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
		HAL_Delay(1000);
	}*/
	if(RS485_TRANSMITTER.data_update == 1){//проверяем флаг апдейта
		crc = (RS485_TRANSMITTER.CRC16[1] << 8) + RS485_TRANSMITTER.CRC16[0];
		if(CRC16_calc(RS485_TRANSMITTER.frame, 7) == crc){
			HAL_UART_Transmit(&huart4, RS485_TRANSMITTER.frame, 9, HAL_MAX_DELAY);
		}else{
			Error_Handler();
		}
	}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_UART4
                              |RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 48;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 65535;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */
  HAL_TIM_MspPostInit(&htim15);

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DIR1_Pin|DIR2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LD4_Pin|LD5_Pin|LD7_Pin|LD9_Pin
                          |LD10_Pin|LD8_Pin|LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : DIR1_Pin */
  GPIO_InitStruct.Pin = DIR1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIR1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIR2_Pin */
  GPIO_InitStruct.Pin = DIR2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIR2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD5_Pin LD7_Pin LD9_Pin
                           LD10_Pin LD8_Pin LD6_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD5_Pin|LD7_Pin|LD9_Pin
                          |LD10_Pin|LD8_Pin|LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint8_t run(uint32_t time_acs_ms, uint16_t velocity /*imp/sec*/, TIM_TypeDef * TimX, TIM_HandleTypeDef *htim, uint32_t Channel1, uint32_t Channel2, uint32_t coordinate /*add*/, GPIO_TypeDef * SwitchPort, uint16_t SwitchPin){
	//РАЗГОН
	//уставка для счетчика
	uint16_t SP = 1000000/velocity;//частота инкреммента / скорость
	//расчет ускорения
	//расстояние за время ускорения (шагов) - перевод в шагов в мс
	uint16_t acs_dist = (velocity*time_acs_ms)/2000; /* трапецивидный профиль /‾\ => получим на ускорении прямоугольный ипеугольник, тогда v*tacs/2 */
	//Расчитываем шаг скорости на один шаг двигателя
	uint16_t StartSpeed_count = 5000;
	uint16_t Delta_count = (StartSpeed_count - SP)/acs_dist + 1;
	//ДВИЖЕНИЕ
	//Счетчик шагов
	uint32_t step_counter = 0;
	//ОСТАНОВКА
	uint16_t deacs_dist = acs_dist;
	TimX -> ARR  = StartSpeed_count;
	TimX -> CCR1 = StartSpeed_count/2; // Скважность по первому каналу 50%
	TimX -> CCR2 = StartSpeed_count/2; // Скважность по второму каналу 50%
	HAL_TIM_PWM_Start(htim, Channel1);
	HAL_TIM_PWM_Start(htim, Channel2);
	if (coordinate >= acs_dist + deacs_dist){
		while(step_counter != acs_dist/*TimX -> ARR != SP*/ && HAL_GPIO_ReadPin(SwitchPort, SwitchPin) != GPIO_PIN_SET){ // пока period != уставке
			if (TimX->SR & TIM_SR_UIF)
			{
				TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
				TimX->ARR -= Delta_count;
				TimX -> CCR1 = TimX->ARR/2; // Скважность по первому каналу 50%
				TimX -> CCR2 = TimX -> CCR1; // Скважность по второму каналу 50%
				step_counter++;
			}
			if(TimX -> ARR < SP){
				TimX -> ARR = SP;
			}
		}
		while(step_counter != coordinate - deacs_dist && HAL_GPIO_ReadPin(SwitchPort, SwitchPin) != GPIO_PIN_SET){
			if (TimX->SR & TIM_SR_UIF)
			{
				TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
				step_counter++;
			}
		}
		while(step_counter != coordinate && HAL_GPIO_ReadPin(SwitchPort, SwitchPin) != GPIO_PIN_SET){
			if (TimX->SR & TIM_SR_UIF)
			{
				TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
				TimX->ARR += Delta_count;
				TimX -> CCR1 = TimX->ARR/2; // Скважность по первому каналу 50%
				TimX -> CCR2 = TimX -> CCR1; // Скважность по второму каналу 50%
				step_counter++;
			}
			if(TimX -> ARR > 5000){
				TimX -> ARR = 5000;
			}
		}
		HAL_TIM_PWM_Stop(htim, Channel1);
		HAL_TIM_PWM_Stop(htim, Channel2);
		if (HAL_GPIO_ReadPin(SwitchPort, SwitchPin) == GPIO_PIN_SET) return 1;
		if (HAL_GPIO_ReadPin(SwitchPort, SwitchPin) == GPIO_PIN_SET) return 0;
	}else{
		return 0;
	}
}
uint8_t direction(uint8_t axis /*0-x 1-y*/, uint8_t dir){
	if(axis == X && dir == pos){
		HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_SET);
	}
	if(axis == X && dir == neg){
		HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_RESET);
	}
	if(axis == Y && dir == pos){
		HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_RESET);
	}
	if(axis == Y && dir == neg){
		HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_SET);
	}
}
/*uint8_t scan(uint32_t width, uint32_t height, uint8_t N){
	for(uint8_t i = 0; i <= N; i++){
		direction(X, pos);
		run(250, 2500, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, width);
		direction(Y, pos);
		run(250, 2500, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, height);
		direction(X, neg);
		run(250, 2500, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, width);
		direction(Y, pos);
		run(250, 2500, TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, height);
	}
}*/

uint8_t homing(TIM_TypeDef * TimX, TIM_HandleTypeDef *htim, uint32_t Channel1, uint32_t Channel2, GPIO_TypeDef * SwitchPort, uint16_t SwitchPin){
	uint32_t step_counter = 0;
	TimX -> ARR  = 5000;
	TimX -> CCR1 = 2500; // Скважность по первому каналу 50%
	TimX -> CCR2 = 2500; // Скважность по второму каналу 50%
	//запуск движения к точке (0;0)
	direction(X, neg);
	HAL_TIM_PWM_Start(htim, Channel1);
	HAL_TIM_PWM_Start(htim, Channel2);
	while(HAL_GPIO_ReadPin(SwitchPort, SwitchPin) != GPIO_PIN_SET && step_counter < 8000){
		if (TimX->SR & TIM_SR_UIF)
		{
			TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
			step_counter++;
		}
	}
	if (step_counter >= 8000){
		HAL_TIM_PWM_Stop(htim, Channel1);
		HAL_TIM_PWM_Stop(htim, Channel2);
		Error_Handler();
	}
	step_counter = 0;
	HAL_TIM_PWM_Stop(htim, Channel1);
	HAL_TIM_PWM_Stop(htim, Channel2);
	HAL_Delay(100);
	HAL_TIM_PWM_Start(htim, Channel1);
	HAL_TIM_PWM_Start(htim, Channel2);
	//Отъезжаем на 250 от концевика (они зацеплены последовательно)
	direction(X, pos);
	while(step_counter != 250){
		if (TimX->SR & TIM_SR_UIF)
		{
			TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
			step_counter++;
		}
	}
	HAL_TIM_PWM_Stop(htim, Channel1);
	HAL_TIM_PWM_Stop(htim, Channel2);
	HAL_Delay(100);
	step_counter = 0;
	//Едем до концевика по оси Y
	direction(Y, neg);
	HAL_TIM_PWM_Start(htim, Channel1);
	HAL_TIM_PWM_Start(htim, Channel2);
	while(HAL_GPIO_ReadPin(SwitchPort, SwitchPin) != GPIO_PIN_SET && step_counter < 8000){
		if (TimX->SR & TIM_SR_UIF)
		{
			TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
			step_counter++;
		}
	}
	if (step_counter >= 8000){
		HAL_TIM_PWM_Stop(htim, Channel1);
		HAL_TIM_PWM_Stop(htim, Channel2);
		Error_Handler();
	}
	HAL_TIM_PWM_Stop(htim, Channel1);
	HAL_TIM_PWM_Stop(htim, Channel2);
	HAL_Delay(100);
	step_counter = 0;
	direction(Y, pos);
	HAL_TIM_PWM_Start(htim, Channel1);
	HAL_TIM_PWM_Start(htim, Channel2);
	while(step_counter != 250){
		if (TimX->SR & TIM_SR_UIF)
		{
			TimX->SR &= ~TIM_SR_UIF; //сброс флага прерывания
			step_counter++;
		}
	}
	HAL_TIM_PWM_Stop(htim, Channel1);
	HAL_TIM_PWM_Stop(htim, Channel2);
	return 1;
}
uint16_t CRC16_calc(uint8_t* data, size_t length) {
    uint16_t crc = 0x0000; // Начальное значение (может быть 0xFFFF)

    for (size_t i = 0; i < length; i++) {
        // XOR текущего байта со старшим байтом CRC
        crc ^= ((uint16_t)data[i] << 8);

        // Обрабатываем каждый бит байта
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                // Если старший бит = 1, сдвигаем и XOR с полиномом
                crc = (crc << 1) ^ 0x8005;
            } else {
                // Иначе просто сдвигаем
                crc <<= 1;
            }
        }
    }

    return crc;
}
uint8_t DataRecive(cmd_uart* structure, uint8_t symbol){
	//прием байта адреса устройства
	structure->frame[structure->ccnt] = symbol;
	if (structure->ccnt == 0){
		structure->dev_addr = symbol;
	}
	//прием двух байт команды
	if (structure -> ccnt == 1){
		structure->cmd[0] = symbol;
	}
	if (structure -> ccnt == 2){
		structure->cmd[1] = symbol;
	}
	//прием двух байт адреса регистра
	if (structure -> ccnt == 3){
		structure-> reg[0] = symbol;
	}
	if (structure -> ccnt == 4){
		structure-> reg[1] = symbol;
	}
	//прием двух байт данных
	if (structure -> ccnt == 5){
		structure-> value[0] = symbol;
	}
	if (structure -> ccnt == 6){
		structure-> value[1] = symbol;
	}
	//прием CRC
	if (structure -> ccnt == 7){
		structure-> CRC16[0] = symbol;
	}
	if (structure -> ccnt == 8){
		structure-> CRC16[1] = symbol;
	}
	structure->ccnt++;
	if(structure->ccnt == 9) {
		structure->ccnt = 0;
		structure->data_update = 1;
	}
}
uint8_t proceed(station * station_structure, cmd_uart * RS485_structure){
	if(RS485_structure->dev_addr == 52){
		if(RS485_structure ->cmd[0] == 0){ //проверяем первый 0
			if(RS485_structure ->cmd[1] == 1){ //запись
				if(RS485_structure->reg[0] == 0){ //проверяем, что первый 0 в адресе регистра
					switch(RS485_structure->reg[1]){
					case 1:
						station_structure->velocity = (RS485_TRANSMITTER.value[1] << 8) + RS485_TRANSMITTER.value[0];
					case 2:
						station_structure->time_acs = (RS485_TRANSMITTER.value[1] << 8) + RS485_TRANSMITTER.value[0];
					case 3:
						station_structure->coordinate = (RS485_TRANSMITTER.value[1] << 8) + RS485_TRANSMITTER.value[0];
					case 4:
						station_structure->status = 1;
						homing(TIM15, &htim15, TIM_CHANNEL_1, TIM_CHANNEL_2, GPIOA, GPIO_PIN_10);
						station_structure->status = 0;
					case 7:
						station_structure->axis = (RS485_TRANSMITTER.value[1]);
						station_structure->dir = (RS485_TRANSMITTER.value[0]);
					case 8:
						station_structure->status = 1;
						direction(station_structure->axis, station_structure->dir);
						run(station_structure->time_acs, station_structure->velocity, TIM15, &htim15 , TIM_CHANNEL_1, TIM_CHANNEL_2, station_structure->coordinate, GPIOA, GPIO_PIN_10);
						station_structure->status = 0;
					}
				}
				if(RS485_structure->reg[0] == 1){
					switch(RS485_structure->reg[1]){
					case 0:
						station_structure->width = (RS485_TRANSMITTER.value[1] << 8) + RS485_TRANSMITTER.value[0];
					case 1:
						station_structure->height = (RS485_TRANSMITTER.value[1] << 8) + RS485_TRANSMITTER.value[0];
					case 2:
						station_structure-> step_number = RS485_TRANSMITTER.value[0];
					case 3:
						Error_Handler();
					}
				}
			}
			if(RS485_structure ->cmd[1] == 4){ //чтение

			}
		}
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
