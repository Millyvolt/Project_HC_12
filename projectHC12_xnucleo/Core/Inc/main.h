/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define USER_button_Pin GPIO_PIN_13
#define USER_button_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_5
#define LED4_GPIO_Port GPIOC
#define Key3_Pin GPIO_PIN_10
#define Key3_GPIO_Port GPIOB
#define HC12_SET_Pin GPIO_PIN_7
#define HC12_SET_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOC
#define Keys_common_Pin GPIO_PIN_8
#define Keys_common_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_15
#define Buzzer_GPIO_Port GPIOA
#define Key2_Pin GPIO_PIN_3
#define Key2_GPIO_Port GPIOB
#define Key4_Pin GPIO_PIN_4
#define Key4_GPIO_Port GPIOB
#define Key1_Pin GPIO_PIN_5
#define Key1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */




#define	RS_I2C				0x01
#define	RW_I2C				0x02
#define	E_I2C					0x04
#define	LED_I2C				0x08
#define	D4_I2C				0x10
#define	D5_I2C				0x20
#define	D6_I2C				0x40
#define	D7_I2C				0x80

#define	GOR_ERROR			0x01

#define	TIME_10_MIN		60000
#define	TIME_20_MIN		120000
#define	TIME_30_MIN		180000

//
#define	PACKET_COUNTER	101
#define	GOR_LEFT_STATE	15
#define	GOR_RIGHT_STATE	79

#define	KEY1_PRESS		0x01
#define	KEY2_PRESS		0x02
#define	KEY3_PRESS		0x04
#define	KEY4_PRESS		0x08


enum Buzzer_state
{
	BUZZER_ON,
	BUZZER_OFF
};

enum Screen_state
{
	SCREEN_ON,
	SCREEN_OFF
};


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
