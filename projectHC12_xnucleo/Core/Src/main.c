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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */




#include "lcd_led.h"



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
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

osThreadId defaultTaskHandle;
osThreadId KeysHandle;
/* USER CODE BEGIN PV */




uint8_t 	but_press_counter=0, packets_old=0, keys=0;
uint16_t	address=0x4E;
uint32_t	time=0;
enum		Buzzer_state	buzzer_state=BUZZER_ON;
enum 		Screen_state	screen_state=SCREEN_ON;

extern	volatile	uint8_t		gor_left_error, gor_right_error, counter_led4, packet_counter;

osThreadId	errorTaskHandle;
osThreadId	userButtonTaskHandle;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void const * argument);
void KeysTask(void const * argument);

/* USER CODE BEGIN PFP */




void	dspl_packets(void);
void	dspl_static_info(void);

void	ErrorIndicateTask(void const * argument);
void	UserButtonTask(void const * argument);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */



	USART1->CR1 |= USART_CR1_RXNEIE;
	
	delay_init();
	delay_ms(3);	//event generate in delay_init doesn't help
	
	delay_ms(100);
//	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
//	delay_ms(1000);

	
	display_2004_i2c_init(&hi2c1);
	
	
	dspl_static_info();

	dspl_packets();



  /* USER CODE END 2 */

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Keys */
  osThreadDef(Keys, KeysTask, osPriorityNormal, 0, 128);
  KeysHandle = osThreadCreate(osThread(Keys), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  
	
	
//	osThreadDef(errorTask, ErrorIndicateTask, osPriorityNormal, 0, 128);
//  errorTaskHandle = osThreadCreate(osThread(errorTask), NULL);

		osThreadDef(buttonTask, UserButtonTask, osPriorityNormal, 0, 128);
		userButtonTaskHandle = osThreadCreate(osThread(buttonTask), NULL);
	
	
	//vTaskSuspend(KeysHandle);
	
	
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|Keys_common_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED4_Pin|LED3_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HC12_SET_GPIO_Port, HC12_SET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : USER_button_Pin */
  GPIO_InitStruct.Pin = USER_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(USER_button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin Keys_common_Pin Buzzer_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|Keys_common_Pin|Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED4_Pin HC12_SET_Pin LED3_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED4_Pin|HC12_SET_Pin|LED3_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Key3_Pin Key2_Pin Key4_Pin Key1_Pin */
  GPIO_InitStruct.Pin = Key3_Pin|Key2_Pin|Key4_Pin|Key1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */



void	dspl_packets(void)
{
	write_cmd_i2c(&hi2c1, D7_I2C|PACKET_COUNTER);
	write_data_i2c(&hi2c1, packets_old/100+48);
	write_data_i2c(&hi2c1, packets_old%100/10+48);
	write_data_i2c(&hi2c1, packets_old%100%10+48);
}

void	dspl_static_info(void)
{
	write_cmd_i2c(&hi2c1, D7_I2C|LINE_1_START);
	write_data_i2c(&hi2c1, 's');
	write_data_i2c(&hi2c1, 'k');
	write_data_i2c(&hi2c1, 'l');
	write_data_i2c(&hi2c1, 'a');
	write_data_i2c(&hi2c1, 'd');
	write_data_i2c(&hi2c1, ' ');
	write_data_i2c(&hi2c1, 'l');
	write_data_i2c(&hi2c1, 'e');
	write_data_i2c(&hi2c1, 'v');
	write_data_i2c(&hi2c1, 'a');
	write_data_i2c(&hi2c1, 'y');
	write_data_i2c(&hi2c1, 'a');
	write_cmd_i2c(&hi2c1, D7_I2C|LINE_2_START);
	write_data_i2c(&hi2c1, 's');
	write_data_i2c(&hi2c1, 'k');
	write_data_i2c(&hi2c1, 'l');
	write_data_i2c(&hi2c1, 'a');
	write_data_i2c(&hi2c1, 'd');
	write_data_i2c(&hi2c1, ' ');
	write_data_i2c(&hi2c1, 'p');
	write_data_i2c(&hi2c1, 'r');
	write_data_i2c(&hi2c1, 'a');
	write_data_i2c(&hi2c1, 'v');
	write_data_i2c(&hi2c1, 'a');
	write_data_i2c(&hi2c1, 'y');
	write_data_i2c(&hi2c1, 'a');
	write_cmd_i2c(&hi2c1, D7_I2C|(LINE_3_START+6));
	write_data_i2c(&hi2c1, '1');
	write_data_i2c(&hi2c1, '-');
	write_data_i2c(&hi2c1, 'm');
	write_data_i2c(&hi2c1, 'u');
	write_data_i2c(&hi2c1, 't');
	write_data_i2c(&hi2c1, 'e');
	write_cmd_i2c(&hi2c1, D7_I2C|LINE_4_START);
	write_data_i2c(&hi2c1, '2');
	write_data_i2c(&hi2c1, '-');
	write_data_i2c(&hi2c1, 'l');
	write_data_i2c(&hi2c1, 'e');
	write_data_i2c(&hi2c1, 'd');
	write_data_i2c(&hi2c1, 'o');
	write_data_i2c(&hi2c1, 'n');
	write_data_i2c(&hi2c1, ' ');
	write_data_i2c(&hi2c1, '3');
	write_data_i2c(&hi2c1, '-');
	write_data_i2c(&hi2c1, 'l');
	write_data_i2c(&hi2c1, 'e');
	write_data_i2c(&hi2c1, 'd');
	write_data_i2c(&hi2c1, 'o');
	write_data_i2c(&hi2c1, 'f');
	write_data_i2c(&hi2c1, 'f');
}

void	ErrorIndicateTask(void const * argument)
{
		for(;;)
		{
		if(gor_left_error)
		{
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			osDelay(100);
		}
		else
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		
		if(gor_right_error)
		{
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			osDelay(100);
		}
		else
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		
		osDelay(100);
		}
}

void	UserButtonTask(void const * argument)
{
	for(;;)
	{
			if(gor_left_error||gor_right_error)
			{
					if(buzzer_state==BUZZER_ON)
					{
						if( HAL_GPIO_ReadPin(USER_button_GPIO_Port, USER_button_Pin) )		
						{																			//not pressed
							but_press_counter = 0;		//for debounce
						}
						else						
						{																			//pressed
							but_press_counter += 1;
							if(but_press_counter==7)
							{
								buzzer_state = BUZZER_OFF;
								HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
								but_press_counter = 0;
								time = TIME_30_MIN;
							}
						}
					}
					
					if(buzzer_state==BUZZER_OFF)
					{
						time -= 1;
						if(!time)
							buzzer_state = BUZZER_ON;
					}
					
					osDelay(10);
			}
			else
			{
//					if(time)
//						time = 0;
//					if(buzzer_state==BUZZER_OFF)
//						buzzer_state = BUZZER_ON;
				
					osDelay(50);
			}
	}
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
		


  for(;;)
  {
		osDelay(100);
		
		if(gor_left_error)
		{
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			if(buzzer_state==BUZZER_ON)
				HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			write_cmd_i2c(&hi2c1, D7_I2C|GOR_LEFT_STATE);
			write_data_i2c(&hi2c1, 'e');
			write_data_i2c(&hi2c1, 'r');
			osDelay(100);
		}
		else
		{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
			write_cmd_i2c(&hi2c1, D7_I2C|GOR_LEFT_STATE);
			write_data_i2c(&hi2c1, 'o');
			write_data_i2c(&hi2c1, 'k');
			osDelay(200);
		}
		
		if(gor_right_error)
		{
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			if(buzzer_state==BUZZER_ON)
				HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			write_cmd_i2c(&hi2c1, D7_I2C|GOR_RIGHT_STATE);
			write_data_i2c(&hi2c1, 'e');
			write_data_i2c(&hi2c1, 'r');
			osDelay(100);
		}
		else
		{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
			write_cmd_i2c(&hi2c1, D7_I2C|GOR_RIGHT_STATE);
			write_data_i2c(&hi2c1, 'o');
			write_data_i2c(&hi2c1, 'k');
			osDelay(200);
		}
		
		osDelay(200);
		
		if(packets_old!=packet_counter)
		{
			packets_old = packet_counter;
			dspl_packets();
		}
		
		if(keys&KEY2_PRESS)
		{
			uint8_t tmp;
			tmp = LED_I2C;												//led on
			HAL_I2C_Master_Transmit(&hi2c1, address, &tmp, 1, 1000);
			screen_state = SCREEN_ON;
			keys &= ~KEY2_PRESS;
		}
		if(keys&KEY3_PRESS)
		{
			uint8_t tmp;
			tmp = 0;													//led off
			HAL_I2C_Master_Transmit(&hi2c1, address, &tmp, 1, 1000);
			screen_state = SCREEN_OFF;
			keys &= ~KEY3_PRESS;
		}
		if(keys&KEY1_PRESS)												//mute
		{
			if(buzzer_state==BUZZER_ON)
			{
				buzzer_state = BUZZER_OFF;
				HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
				time = TIME_30_MIN;
				keys &= ~KEY1_PRESS;
			}
		}
		
		osDelay(300);
		
//		HAL_GPIO_WritePin(HC12_SET_GPIO_Port, HC12_SET_Pin, GPIO_PIN_RESET);
//		osDelay(200);
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = 'A';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = 'T';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = '+';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = 'R';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = 'X';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		osDelay(3000);
//		USART1->DR = 'A';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = 'T';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = '+';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		USART1->DR = 'V';
//		while(!(USART1->SR&USART_SR_TXE))
//			;
//		osDelay(200);
//		HAL_GPIO_WritePin(HC12_SET_GPIO_Port, HC12_SET_Pin, GPIO_PIN_SET);

  }
	
	
	
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_KeysTask */
/**
* @brief Function implementing the Keys thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KeysTask */
void KeysTask(void const * argument)
{
  /* USER CODE BEGIN KeysTask */
  /* Infinite loop */
  for(;;)
  {
		if(!HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin))
			keys |= KEY1_PRESS;
		if(!HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin))
			keys |= KEY2_PRESS;
		if(!HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin))
			keys |= KEY3_PRESS;
	  
		osDelay(100);
  }
  /* USER CODE END KeysTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
