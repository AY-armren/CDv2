/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define DIR1_Pin GPIO_PIN_0
#define DIR1_GPIO_Port GPIOA
#define DIR2_Pin GPIO_PIN_1
#define DIR2_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MISOA7_Pin GPIO_PIN_7
#define SPI1_MISOA7_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOE
#define LD5_Pin GPIO_PIN_10
#define LD5_GPIO_Port GPIOE
#define LD7_Pin GPIO_PIN_11
#define LD7_GPIO_Port GPIOE
#define LD9_Pin GPIO_PIN_12
#define LD9_GPIO_Port GPIOE
#define LD10_Pin GPIO_PIN_13
#define LD10_GPIO_Port GPIOE
#define LD8_Pin GPIO_PIN_14
#define LD8_GPIO_Port GPIOE
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOE
#define DM_Pin GPIO_PIN_11
#define DM_GPIO_Port GPIOA
#define DP_Pin GPIO_PIN_12
#define DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
//инициализация всего того, что будет происходить в прерываниях
typedef struct{
	uint8_t ccnt; //счетчик
	uint8_t dev_addr; // адрес усройства
	uint8_t cmd[2];	  // команда
	uint8_t reg[2];	  // регистр
	uint8_t value[2]; // значение
	uint8_t CRC16[2]; // CRC
	uint8_t data_update; // флаг заполнения данных
	uint8_t frame[9];    //общий фрейм
}cmd_uart; //структура для команды
extern cmd_uart RS485_TRANSMITTER; //объявление структуры
uint16_t CRC16_calc(uint8_t* data, size_t length);
uint8_t DataRecive(cmd_uart* structure, uint8_t symbol);
typedef struct{
	uint16_t velocity;   //скорость
	uint16_t time_acs;   //время разгона
	uint16_t coordinate; //счетчик шагов
	uint8_t axis;        //ось 0 - Y, 1 - X
	uint8_t dir;		 //направление 0 - +     1 - -
	uint16_t status;    //status (какая команда выполняется)
	uint8_t x_home;		// статус определения домашней позиции по X
	uint8_t y_home;		// статус определения домашней позиции по Y
	uint8_t cmd;
}station;
extern uint8_t newline_char;
uint8_t direction(uint8_t axis /*0-x 1-y*/, uint8_t dir);
extern cmd_uart RS485_TRANSMITTER;
extern uint8_t rx_buffer;
extern uint16_t crc;
extern station station_struct;

extern uint16_t SP; //уставка для регистра
extern uint16_t acs_dist; //расстояние на ускорение
extern uint16_t StartSpeed_count; //начальная скороть
extern uint16_t Delta_count; //прирост в счетчик
extern uint32_t step_counter; // счечтик шагов
extern uint8_t dist_x; //флаг достижения концевика при хоуминге по X
extern uint8_t dist_y; //флаг достижения концевика при хоуминге по Y
extern uint16_t crc_check;
uint8_t proceed(station * structure, cmd_uart * RS485_structure);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
