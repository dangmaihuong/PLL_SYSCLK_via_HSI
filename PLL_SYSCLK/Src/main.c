/*
 * main.c
 * Author: Huong Dang
 * Date: May 12nd, 2020
 * purpose: Learning using peripheral UART
 * Content: Send character over UART to MCU. MCU convert all lower case
 * into upper case and send it back to user
 *
 */

#include <string.h>
#include "main.h"
#include <stdio.h>

#define SYS_CLOCK_FREQ_24 24
#define SYS_CLOCK_FREQ_16 16
#define SYS_CLOCK_FREQ_32 32

void UART2_Init(void);
void Error_handler(void);
void SystemClockConfig(uint8_t clock_freq);

UART_HandleTypeDef huart2;


int main(void)
{

	char msg[100];
	HAL_Init();


	//To change the frequency of HCLK, using 3 option defined values of SYS_CLOCK_FREQ to run the function
	//always keep a systemclockconfig the second
	SystemClockConfig(SYS_CLOCK_FREQ_24);

	UART2_Init();

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "HCLK: %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "APB1: %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "APB2: %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);


	while(1);

	return 0;
}


void SystemClockConfig(uint8_t clock_freq){

	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint8_t Flatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON; // by default it is on
	osc_init.HSICalibrationValue = 16; //by default it is 16
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch (clock_freq){
	case SYS_CLOCK_FREQ_24:{
		osc_init.PLL.PLLMUL = RCC_PLL_MUL6;
		osc_init.PLL.PLLDIV = RCC_PLL_DIV4;

		clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
				| RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

		Flatency = FLASH_ACR_LATENCY_Msk;
		break;
	}
	case SYS_CLOCK_FREQ_16:{
		osc_init.PLL.PLLMUL = RCC_PLL_MUL4;
		osc_init.PLL.PLLDIV = RCC_PLL_DIV4;

		clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
				| RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

		Flatency = FLASH_ACR_LATENCY_Pos;
		break;
	}
	case SYS_CLOCK_FREQ_32:{
		osc_init.PLL.PLLMUL = RCC_PLL_MUL4;
		osc_init.PLL.PLLDIV = RCC_PLL_DIV2;

		clk_init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
				| RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

		Flatency = FLASH_ACR_LATENCY_Msk;
		break;
	}
	default:
		return;
	}

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_handler();
	}

	if (HAL_RCC_ClockConfig(&clk_init, Flatency) != HAL_OK){
		Error_handler();
	}



	//SYSTICK configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}





void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_handler();
	}


}




void Error_handler(void)
{
	while(1);
}

