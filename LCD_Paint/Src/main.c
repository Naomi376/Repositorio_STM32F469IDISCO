#include "stm32f4xx_hal.h"
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_lcd.h"
#include "stm32469i_discovery_sdram.h"
#include "string.h"

UART_HandleTypeDef uartres;

void SystemClock_Config(void);
void Error(void);
void MX_USART3_UART_Init(void);



int main(void)
{
	HAL_Init();
	SystemClock_Config();

	if(BSP_SDRAM_Init()!= SDRAM_OK)
	{
		Error();
	}


	if(BSP_LCD_Init()!=LCD_OK)
	{
		Error();
	}

	BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE);
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLUE);


    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);


    BSP_LED_Init(LED3);
    BSP_LED_Off(LED3);

    //UART

    MX_USART3_UART_Init();

    //Variables recepción
    uint8_t recepcion;
    char arreglo[64];
    uint8_t direccionarreglo = 0;
    memset(arreglo, 0, sizeof(arreglo));


	while (1)
	    {

//		 if (HAL_UART_Transmit(&uartres, (uint8_t*)"STM32 listo\r\n", 14, 100) == HAL_OK)
//		    {
//		        BSP_LED_Toggle(LED3);
//		    }
//		    else
//		    {
//		        BSP_LED_On(LED3);
//		    }
//		 HAL_Delay(500);


		    if (HAL_UART_Receive(&uartres, &recepcion, 1, 10) == HAL_OK)
		    {

		           if (recepcion == '\r')
		        {

		             arreglo[direccionarreglo] ="\0";
		             BSP_LCD_Clear(LCD_COLOR_BLUE);
		             BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Probando RX Y TX");
		             BSP_LCD_DisplayStringAtLine(5, (uint8_t*)arreglo);
		             memset(arreglo, 0, sizeof(arreglo));
		             direccionarreglo= 0;
		        }

		        else
		        {

		            if (direccionarreglo < 63)
		            {
		                arreglo[direccionarreglo] = recepcion;
		                direccionarreglo++;
		            }
		        }
		    }



}
}



void Error(void)
{

  BSP_LED_On(LED4);
  while(1)
  {
  }
}






 void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
  RCC_OscInitStruct.PLL.PLLM = 25;
#else
  RCC_OscInitStruct.PLL.PLLM = 8;
#endif
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}


 void MX_USART3_UART_Init(void)
{
	    // Relojes para el GPIO (GPIOB), para USART3
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	    __HAL_RCC_USART3_CLK_ENABLE();

	    GPIO_InitTypeDef GPIO_InitStruct = {0};
	    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_PULLUP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    uartres.Instance = USART3;
	    uartres.Init.BaudRate = 115200;
	    uartres.Init.WordLength = UART_WORDLENGTH_8B;
	    uartres.Init.StopBits = UART_STOPBITS_1;
	    uartres.Init.Parity = UART_PARITY_NONE;
	    uartres.Init.Mode = UART_MODE_TX_RX;
	    uartres.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	    uartres.Init.OverSampling = UART_OVERSAMPLING_16;
	    if (HAL_UART_Init(&uartres) != HAL_OK)
	        Error();

}




#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


