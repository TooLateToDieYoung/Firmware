/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Common.h"
#include "Buffer.h"
#include "Button.h"
#include "HC05.h"
#include "LSM6DS3.h"
#include "VL53L1X.h"
// #include "VL53L1X_api.h"
#include "SevenSegment.h"
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
  task_t APP_Init(void);
  task_t APP_Task(void);
  void APP_SysTick_Handler(void);
  void APP_TIM4_IRQHandler(void);
  void APP_USART1_IRQHandler(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TEST_Pin LL_GPIO_PIN_13
#define TEST_GPIO_Port GPIOC
#define SCS_Pin LL_GPIO_PIN_4
#define SCS_GPIO_Port GPIOA
#define SCLK_Pin LL_GPIO_PIN_5
#define SCLK_GPIO_Port GPIOA
#define MISO_Pin LL_GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define MOSI_Pin LL_GPIO_PIN_7
#define MOSI_GPIO_Port GPIOA
#define SSE_Pin LL_GPIO_PIN_0
#define SSE_GPIO_Port GPIOB
#define SSD_Pin LL_GPIO_PIN_1
#define SSD_GPIO_Port GPIOB
#define SSP_Pin LL_GPIO_PIN_10
#define SSP_GPIO_Port GPIOB
#define SSC_Pin LL_GPIO_PIN_11
#define SSC_GPIO_Port GPIOB
#define SSG3_Pin LL_GPIO_PIN_12
#define SSG3_GPIO_Port GPIOB
#define SSG2_Pin LL_GPIO_PIN_13
#define SSG2_GPIO_Port GPIOB
#define SSG1_Pin LL_GPIO_PIN_14
#define SSG1_GPIO_Port GPIOB
#define SWFD_Pin LL_GPIO_PIN_15
#define SWFD_GPIO_Port GPIOB
#define MTSR_Pin LL_GPIO_PIN_9
#define MTSR_GPIO_Port GPIOA
#define MRST_Pin LL_GPIO_PIN_10
#define MRST_GPIO_Port GPIOA
#define SSB_Pin LL_GPIO_PIN_3
#define SSB_GPIO_Port GPIOB
#define SSF_Pin LL_GPIO_PIN_4
#define SSF_GPIO_Port GPIOB
#define SCL_Pin LL_GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin LL_GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
#define SSA_Pin LL_GPIO_PIN_8
#define SSA_GPIO_Port GPIOB
#define SSG_Pin LL_GPIO_PIN_9
#define SSG_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0 ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority, \
                                                         4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1 ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority, \
                                                         3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2 ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority, \
                                                         2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3 ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority, \
                                                         1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4 ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority, \
                                                         0 bit  for subpriority */
#endif
  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
