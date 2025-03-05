/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define BTN_INGRESO_Pin GPIO_PIN_0
#define BTN_INGRESO_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define BTN_EGRESO_Pin GPIO_PIN_1
#define BTN_EGRESO_GPIO_Port GPIOB
#define BTN_ENTER_Pin GPIO_PIN_10
#define BTN_ENTER_GPIO_Port GPIOB
#define BTN_NEXT_Pin GPIO_PIN_11
#define BTN_NEXT_GPIO_Port GPIOB
#define BTN_ON_Pin GPIO_PIN_12
#define BTN_ON_GPIO_Port GPIOB
#define SWITCH_BARINF_Pin GPIO_PIN_13
#define SWITCH_BARINF_GPIO_Port GPIOB
#define SWITCH_OFF_Pin GPIO_PIN_14
#define SWITCH_OFF_GPIO_Port GPIOB
#define LED_SYSCTRL_DIS_Pin GPIO_PIN_10
#define LED_SYSCTRL_DIS_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED_MIN_VEL_Pin GPIO_PIN_4
#define LED_MIN_VEL_GPIO_Port GPIOB
#define LED_SYSCTRL_ACT_Pin GPIO_PIN_5
#define LED_SYSCTRL_ACT_GPIO_Port GPIOB
#define LED_MAX_VEL_Pin GPIO_PIN_8
#define LED_MAX_VEL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
