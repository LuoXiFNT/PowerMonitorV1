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
#include "stm32g0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_RUN_Pin GPIO_PIN_15
#define LED_RUN_GPIO_Port GPIOC
#define CURRENT_Pin GPIO_PIN_0
#define CURRENT_GPIO_Port GPIOA
#define VOLTAGE_Pin GPIO_PIN_1
#define VOLTAGE_GPIO_Port GPIOA
#define UNIT_SW_Pin GPIO_PIN_4
#define UNIT_SW_GPIO_Port GPIOA
#define UNIT_SW_EXTI_IRQn EXTI4_15_IRQn
#define CLEAR_SW_Pin GPIO_PIN_5
#define CLEAR_SW_GPIO_Port GPIOA
#define CLEAR_SW_EXTI_IRQn EXTI4_15_IRQn
#define HOLD_SW_Pin GPIO_PIN_6
#define HOLD_SW_GPIO_Port GPIOA
#define HOLD_SW_EXTI_IRQn EXTI4_15_IRQn
#define MENU_SW_Pin GPIO_PIN_7
#define MENU_SW_GPIO_Port GPIOA
#define MENU_SW_EXTI_IRQn EXTI4_15_IRQn
#define I2C2_SCL_Pin GPIO_PIN_11
#define I2C2_SCL_GPIO_Port GPIOA
#define I2C2_SDA_Pin GPIO_PIN_12
#define I2C2_SDA_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
