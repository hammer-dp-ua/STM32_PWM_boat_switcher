#include "main.h"

IWDG_HandleTypeDef hiwdg;

/* Timer handler declaration */
TIM_HandleTypeDef timHandle;

/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef sConfig;

/* Slave configuration structure */
TIM_SlaveConfigTypeDef sSlaveConfig;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM3_Init(void);

/* Captured Value */
__IO uint32_t uwIC2Value = 0;
/* Duty Cycle Value */
__IO uint32_t uwDutyCycle = 0;
/* Frequency Value */
__IO uint32_t uwFrequency = 0;

/**
  * @brief  Input Capture callback in non blocking mode
  * @param  htim : TIM IC handle
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		/* Get the Input Capture value */
		uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

		if (uwIC2Value != 0) {
			/* Duty cycle computation */
			uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)) * 100) / uwIC2Value;

			/* uwFrequency computation TIM3 counter clock = (RCC_Clocks.HCLK_Frequency) */
			uwFrequency = (HAL_RCC_GetHCLKFreq()) / uwIC2Value;
		} else {
			uwDutyCycle = 0;
			uwFrequency = 0;
		}
	}
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();

	//MX_IWDG_Init();
	MX_TIM3_Init();

	/* Infinite loop */
	while (1) {
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI/2)
  *            SYSCLK(Hz)                     = 8000000
  *            HCLK(Hz)                       = 1000000
  *            AHB Prescaler                  = 8
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure. */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	//HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void) {

	/* USER CODE BEGIN IWDG_Init 0 */

	/* USER CODE END IWDG_Init 0 */

	/* USER CODE BEGIN IWDG_Init 1 */

	/* USER CODE END IWDG_Init 1 */
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
	hiwdg.Init.Window = 156;
	hiwdg.Init.Reload = 156;
	if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN IWDG_Init 2 */

	/* USER CODE END IWDG_Init 2 */

}

static void MX_TIM3_Init(void) {
	/*##-1- Configure the TIM peripheral #######################################*/
	/* ---------------------------------------------------------------------------
	TIM3 configuration: PWM Input mode

	In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1), since APB1 prescaler is 1.
	TIM3CLK = PCLK1
	PCLK1 = HCLK => TIM3CLK = HCLK = SystemCoreClock

	External Signal Frequency = TIM3 counter clock / TIM3_CCR2 in Hz.

	External Signal DutyCycle = (TIM3_CCR1*100)/(TIM3_CCR2) in %.
	--------------------------------------------------------------------------- */
	timHandle.Instance = TIM3;
	timHandle.Init.Prescaler = 0;
	timHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	timHandle.Init.Period = 65535;
	timHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_IC_Init(&timHandle) != HAL_OK) {
		Error_Handler();
	}

	/*##-2- Configure the Input Capture channels ###############################*/
	/* Common configuration */
	sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	sConfig.ICFilter = 0;

	/*##-2- Configure the Input Capture channel ################################*/
	/* Configure the Input Capture of channel 1 */
	sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&timHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}

	/* Configure the Input Capture of channel 2 */
	sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&timHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}

	/*##-3- Configure the slave mode ###########################################*/
	/* Select the slave Mode: Reset Mode  */
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
	sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
	sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_NONINVERTED;
	sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
	sSlaveConfig.TriggerFilter = 0;
	if (HAL_TIM_SlaveConfigSynchro(&timHandle, &sSlaveConfig) != HAL_OK) {
		Error_Handler();
	}

	/*##-4- Start the Input Capture in interrupt mode ##########################*/
	if (HAL_TIM_IC_Start_IT(&timHandle, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}

	/*##-5- Start the Input Capture in interrupt mode ##########################*/
	if (HAL_TIM_IC_Start_IT(&timHandle, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin : PA5 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
