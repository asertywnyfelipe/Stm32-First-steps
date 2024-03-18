/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stm32F4xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//typedef void (*ButtonPressCallback)(TIM_HandleTypeDef *);
//typedef void (*ButtonReleaseCallback)(TIM_HandleTypeDef *);

typedef void (*ButtonCallback)(TIM_HandleTypeDef *);
typedef void (*TimerCallback)(TIM_HandleTypeDef *);

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    ButtonCallback press_callback;
    ButtonCallback release_callback;
} Button;

typedef struct {
    TimerCallback startTimer;
    ButtonCallback stopTimer;
} TimerON;



void startTimer1(TIM_HandleTypeDef *htim) {
}


void stopTimer1(TIM_HandleTypeDef *htim) {
}

TimerON timers[]=
{
		{startTimer1, stopTimer1},
};


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void handleButtonPress1(TIM_HandleTypeDef *htim) {




			uint16_t timer_val;
			timer_val = __HAL_TIM_GET_COUNTER(htim);
			 if (timer_val>=2000)
			 {
			 HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);





			 printf("Wartosc timera: %d\n", timer_val);
			 HAL_Delay(100);
			 }

}

void handleButtonRelease1(TIM_HandleTypeDef *htim) {

			HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
			htim->Instance->CNT = 0;
}

Button buttons[] = {
    {GPIOA, GPIO_PIN_0, handleButtonPress1, handleButtonRelease1},
};

#define NUM_BUTTONS (sizeof(buttons) / sizeof(buttons[0]))


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void handleButtonPress1(void);
//void handleButtonPress2(void);


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
  MX_DMA_Init();
  MX_TIM14_Init();
  MX_USART2_UART_Init();
  MX_TIM13_Init();
  MX_TIM4_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
 // uart_buf_len = sprintf(uart_buf, "Timer test\r\n");
 // HAL_UART_Transmit(&husart2, (uint8_t *)uart_buf, uart_buf_len, 100);
  //Start timer
  HAL_TIM_Base_Start(&htim14);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	 uint32_t dutyCycle = 500; // Example: 50% duty cycle
	 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, dutyCycle);

  while (1)
  {









	  for (int i = 0; i < NUM_BUTTONS; i++)
	  {
	              if (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == 1)
	              {
	                  buttons[i].press_callback(&htim14);
	              }
	              if (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == 0)
	              {
	            	  buttons[i].release_callback(&htim14);
	              }
	    }
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */









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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
