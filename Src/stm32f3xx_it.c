/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t test_var=0;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim15;
extern UART_HandleTypeDef huart4;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 break and TIM15 interrupts.
  */
void TIM1_BRK_TIM15_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM15_IRQn 0 */
  if(station_struct.status == 1){
	  if(step_counter < station_struct.coordinate){
			if(step_counter < acs_dist){
				TIM15->ARR -= Delta_count;
				TIM15->CCR1 = TIM15->ARR/2;
				TIM15->CCR2 = TIM15->CCR1;
				if(TIM15->ARR < SP){
					TIM15->ARR = SP;
					TIM15->CCR1 = SP/2;
					TIM15->CCR2 = TIM15->CCR1;
				}
			}
			if(step_counter >= station_struct.coordinate-acs_dist){
				TIM15->ARR += Delta_count;
				TIM15->CCR1 = TIM15->ARR/2;
				TIM15->CCR2 = TIM15->CCR1;
				if(TIM15->ARR > 5000){
					TIM15->ARR = 5000;
					TIM15->CCR1 = 2500;
					TIM15->CCR2 = 2500;
				}
			}
	  }
	  if(step_counter == station_struct.coordinate || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET){
		  station_struct.status = 0;
		  HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_1);    // останавливаем ШИМ по каналу 1
		  HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_2);
		  HAL_TIM_Base_Stop_IT(&htim15);
		  RS485_TRANSMITTER.frame[5] = 0;
		  if(step_counter != station_struct.coordinate){
			  RS485_TRANSMITTER.frame[6] = 1;
		  }else{
			  RS485_TRANSMITTER.frame[6] = 0;
		  }
		  RS485_TRANSMITTER.frame[0] = 32;
		  HAL_UART_Transmit(&huart4, RS485_TRANSMITTER.frame, 9, HAL_MAX_DELAY);
		  HAL_UART_Transmit(&huart4, &newline_char, 1, HAL_MAX_DELAY);
	  }
  }

  if(station_struct.status == 2){
	  if(dist_x != 1){
		  direction(1, 1);
		  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET){
			  dist_x = 1;
			  direction(1, 0);
			  step_counter = 0;
		  }
	  }
	  if(step_counter == 200 && dist_x == 1){
		  station_struct.x_home = 1;
	  }

	  if(station_struct.x_home == 1|| step_counter == 16000){
		  HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_1);    // останавливаем ШИМ по каналу 1
		  HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_2);
		  HAL_TIM_Base_Stop_IT(&htim15);
		  RS485_TRANSMITTER.frame[5] = 0;
		  if(step_counter != 16000){
			  RS485_TRANSMITTER.frame[6] = 1;
		  }else{
			  RS485_TRANSMITTER.frame[6] = 0;
		  }
		  HAL_UART_Transmit(&huart4, RS485_TRANSMITTER.frame, 9, HAL_MAX_DELAY);
		  HAL_UART_Transmit(&huart4, &newline_char, 1, HAL_MAX_DELAY);
	  }
  }
  if(station_struct.status == 3){
  	  if(dist_y != 1){
  		  direction(0, 1);
  		  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET){
  			  dist_y = 1;
  			  direction(0, 0);
  			  step_counter = 0;
  		  }
  	  }
  	  if(step_counter == 200 && dist_y == 1){
  		  station_struct.y_home = 1;
  	  }

  	  if(station_struct.y_home == 1|| step_counter == 16000){
  		 HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_1);    // останавливаем ШИМ по каналу 1
		 HAL_TIM_PWM_Stop_IT(&htim15, TIM_CHANNEL_2);
		 HAL_TIM_Base_Stop_IT(&htim15);
		 RS485_TRANSMITTER.frame[5] = 0;
		 if(step_counter != 16000){
			 RS485_TRANSMITTER.frame[6] = 1;
		 }else{
			 RS485_TRANSMITTER.frame[6] = 0;
		 }
		 HAL_UART_Transmit(&huart4, RS485_TRANSMITTER.frame, 9, HAL_MAX_DELAY);
		 HAL_UART_Transmit(&huart4, &newline_char, 1, HAL_MAX_DELAY);
  	  }
    }

  TIM15->SR &= ~TIM_SR_UIF; //сброс флага прерывания
  step_counter++;
  /* USER CODE END TIM1_BRK_TIM15_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  HAL_TIM_IRQHandler(&htim15);
  /* USER CODE BEGIN TIM1_BRK_TIM15_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM15_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt / UART4 wake-up interrupt through EXTI line 34.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
  uint8_t received_char = (uint8_t)(UART4-> RDR  & 0xFF);
  DataRecive(&RS485_TRANSMITTER, received_char);//выполняем заполнение команды
  if(RS485_TRANSMITTER.data_update == 1){//проверяем флаг апдейта
	crc = (RS485_TRANSMITTER.CRC16[0] << 8) + RS485_TRANSMITTER.CRC16[1];
	crc_check = CRC16_calc(RS485_TRANSMITTER.frame, 7);
	if(crc == crc_check){
		proceed(&station_struct, &RS485_TRANSMITTER);
		RS485_TRANSMITTER.data_update = 0;
	}else{
		RS485_TRANSMITTER.data_update = 0;
		//Error_Handler();
	}
  }
  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
