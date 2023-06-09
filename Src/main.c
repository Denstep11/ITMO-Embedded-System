/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "kb.h"
#include "sdk_uart.h"
#include "pca9538.h"
#include "oled.h"
#include "fonts.h"
#include "buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIMER_VALUE 1000
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
int KB_Listen( void );
void oled_Write_Number(int number);
void ghul_Action(void);
void oled_Reset(void);
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
  size_t melody_size;
  uint32_t* melody;
  uint32_t* delays;
  int special_action_id;
	uint32_t eva_melody[] = {
	  N_C5, N_DS5, N_F5, N_DS5, N_F5, N_F5, N_F5, N_AS5, N_GS5, N_G5, N_F5, N_G5, 0, N_G5, N_AS5, N_C6, N_F5, N_DS5, N_AS5, N_AS5, N_G5, N_AS5, N_AS5, N_C6, N_C6
	  };
	uint32_t eva_delays[] = {
	  4, 4, 16/3, 16/3, 8, 8, 8, 8, 8, 16, 8, 16/3, 8, 4, 4, 16/3, 16/3, 8, 8, 8, 8, 8, 16/3, 16, 4
	};
  uint32_t tokyo_ghoul_melody[] = {
    N_AS5, N_C6, N_AS5, N_A5, N_G5, N_C6, N_AS5, N_A5, N_G5, N_G5, N_F5, N_DS5, N_DS5, N_F5, N_D5, N_D5, 0, N_D5, N_D5, N_D5, N_D5, N_D6, N_D6, N_C5, N_AS4, N_C5, N_AS4, N_C5, N_AS5, N_A5, N_A5, N_A5, N_AS5, N_AS5
    };
  uint32_t tokyo_ghoul_delays[] = {
    8,     4,   4,      8,    4,    4,    4,    4,     4,    8,   8/3,    8,    4,     8,    8,   4/3, 8,   8,    4,    8,    4,    8,    4,    4,    8,      4,    8,    8,    8,    4,      8,    4,    8,    4
  };
  uint32_t tokyo_ghoul_melody2[] = {
    N_AS5, N_C6, N_AS5, N_A5, N_G5, N_C6, N_AS5, N_A5, N_G5, N_G5, N_F5, N_DS5, N_DS5, N_F5, N_D5, N_D5, 0, N_D5, N_D5, N_D5, N_D5, N_D6, N_D6,  0, N_AS5, N_A5, N_A5, N_A5, N_AS5, N_AS5
  };
  uint32_t tokyo_ghoul_delays2[] = {
    8,     4,   4,      8,    4,    4,    4,    4,     4,    8,   8/3,    8,    4,     8,    8,   4/3, 8,   8,    4,    8,    4,    8,    2,   8/5,  8,    4,      8,    4,    8,    4
  };
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
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  oled_Init();
  Buzzer_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    special_action_id = 0;
	  int number = KB_Listen();
    if (number == 7) 
    {
      melody = tokyo_ghoul_melody;
      delays = tokyo_ghoul_delays;
      melody_size = sizeof(tokyo_ghoul_melody)/sizeof(tokyo_ghoul_melody[0]);
      special_action_id = 1000-7;
    }
    else
    {
      melody = eva_melody;
      delays = eva_delays;
      melody_size = sizeof(eva_melody)/sizeof(eva_melody[0]);
    }
	  for (int i=number; i>=0; i--) {
		  oled_Write_Number(i);
		  HAL_Delay(TIMER_VALUE);
	  }
    if (special_action_id == 1000-7)
      ghul_Action();

	  Buzzer_Play(melody, delays, melody_size);
	  HAL_Delay(TIMER_VALUE);
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
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int KB_Listen( void ) {
	oled_Reset();
	UART_Transmit( (uint8_t*)"Start listening keyboard\n" );
	uint8_t Key, ROW[4] = {ROW1, ROW2, ROW3, ROW4};
	oled_WriteString("Enter your time:", Font_7x10, White);
	int number = 0;
	while( 1 ) {
		number %= 1000;
		oled_Write_Number(number);
		for (int i=0; i<3; i++)
		{
			Key = Check_Row( ROW[i] );
			if (Key == 0x01) {
				UART_Transmit( (uint8_t*)"Right pressed\n" );
				number*=10;
				number += 3+i*3;
			} else if ( Key == 0x02) {
				UART_Transmit( (uint8_t*)"Center pressed\n" );
				number*=10;
				number += 2+i*3;

			} else if ( Key == 0x04) {
				UART_Transmit( (uint8_t*)"Left pressed\n" );
				number*=10;
				number += 1+i*3;
			}
		}
		Key = Check_Row( ROW[3] );
		if (Key == 0x01) {
			UART_Transmit( (uint8_t*)"Last row right pressed. Keyboard was disactivated\n" );
			return number;
		} else if ( Key == 0x02) {
			UART_Transmit( (uint8_t*)"Last row center pressed\n" );
			number*=10;
		} else if ( Key == 0x04) {
			UART_Transmit( (uint8_t*)"Last row left pressed\n" );
			number = 0;
		}
		HAL_Delay(50);
	}
}

void oled_Write_Number(int number)
{
  number%=1000;
	oled_SetCursor(56, 20);
	oled_WriteChar(0x30+(number/100)%10, Font_7x10, White);
	oled_WriteChar(0x30+(number%100)/10, Font_7x10, White);
	oled_WriteChar(0x30+number%10, Font_7x10, White);
	oled_UpdateScreen();
}

void ghul_Action(void)
{
  oled_SetCursor(56, 20);
	oled_WriteString("1000-7", Font_7x10, White);
	oled_UpdateScreen();
}
void oled_Reset(void) {
	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_UpdateScreen();
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
