
#include "main.h"
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	/* Enable GPIO channels Clock */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure  (TIM3_Channel) in Alternate function, push-pull and High speed */
	GPIO_InitStruct.Pin = GPIO_PIN_7; // Pin #13
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*##-2- Configure the NVIC for TIMx #########################################*/

	HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);

	/* Enable the TIM3 global Interrupt */
	HAL_NVIC_EnableIRQ(TIMx_IRQn);
}
