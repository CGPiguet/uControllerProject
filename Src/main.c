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
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "lcd_driver.h"
#include "rgb.h"
#include "flashSave.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int 	ADC_POT_Right;
float 	celsius;
char 	charTemp[32];
float 	temperatureDisplay;

int flagTemp = 0;
int flagDebug = 0;
//bool flagSwitchUpPressed = false;

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
int fputc(int ch, FILE *f) {  ITM_SendChar(ch);  return(ch); }

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_4) {
		if(flagTemp == 2)
			flagTemp = 0;
		else
			flagTemp++;
	}
	if (GPIO_Pin == GPIO_PIN_0) {

		if(flagDebug==1)
			flagDebug = 0;
		else
			flagDebug++;
	}
	if (GPIO_Pin == GPIO_PIN_1)
		Write_Flash(celsius);

}
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
	MX_I2C1_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_ADC1_Init();
	/* USER CODE BEGIN 2 */
	sendData(0xA5);

	// Initialise LCD and show "Welcome" top/left justified
	lcd_init();
	lcd_clear();

	lcd_setLine(127,0,127,31,1);
	lcd_setLine(0,0,0,31,1);
	lcd_show();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		//Initialize the ADC for debug
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		ADC_POT_Right = HAL_ADC_GetValue(&hadc1)/40-40;

		int16_t TwoByteTemp;
		uint8_t byteTemperature[0];

		HAL_I2C_Master_Transmit(&hi2c1, 0x90, 0x00, 1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&hi2c1, 0x91, byteTemperature,2, HAL_MAX_DELAY);
		//		Need to combine the ByteTemp[1] and [0] into one
		TwoByteTemp = ((int16_t)byteTemperature[0]<<4) | (byteTemperature[1]>>4);

		//celsius = TwoByteTemp *0.0625;
		setRGB(celsius);
		//		HAL_Delay(50);
		if(flagDebug==0)
			celsius = TwoByteTemp *0.0625;
		else
			celsius = ADC_POT_Right;

		switch (flagTemp) {
		case 0:
			//Celsius
			lcd_clear();
			if (flagDebug==0)
				lcd_setString(4,4,"Celsius",LCD_FONT_8,false);
			else
				lcd_setString(4,4,"Celsius PotMod",LCD_FONT_8,false);
			temperatureDisplay = celsius;
			break;
		case 1:
			//Fahrenheit
			lcd_clear();
			if (flagDebug==0)
				lcd_setString(4,4,"Fahrenheit",LCD_FONT_8,false);
			else
				lcd_setString(4,4,"Fahrenheit PotMod",LCD_FONT_8,false);
			temperatureDisplay = (celsius*(9/5)) + 32;
			break;
		case 2:
			//Kelvin
			lcd_clear();
			if (flagDebug==0)
				lcd_setString(4,4,"Kelvin",LCD_FONT_8,false);
			else
				lcd_setString(4,4,"Kelvin PotMod",LCD_FONT_8,false);
			temperatureDisplay = celsius + 273.15;
			break;
		}
		lcd_setLine(127,0,127,31,1);
		lcd_setLine(0,0,0,31,1);
		sprintf(charTemp,"%.2f", temperatureDisplay);
		lcd_setString(4,18,charTemp,LCD_FONT_8,false);
		lcd_show();

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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
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
