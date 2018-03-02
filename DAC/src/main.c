/**
  ******************************************************************************
  * @file    main.c
  * @author  Team Mu
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"

#define SD_I2S_ALT_FUNC GPIO_AF_SPI1
#define SD_I2C_ALT_FUNC GPIO_AF_I2C1

#define SD_I2S_WS 4
#define SD_I2S_CK 5
#define SD_I2S_SD 6

#define SD_I2C_SCL 8
#define SD_I2C_SDA 9

void init();
void initI2sPins();
void initI2c();
void initCodec();

int main(void)
{
	init();
	for(;;);
}

/**
 * Initializes all the components of the system.
 */
void init()
{
	// Enable RCC clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

	initI2s();
	initI2c();
}

/**
 * Initializes the I2S component of the system, designed for use with the codec
 * for transmission of audio data.
 */
void initI2sPins()
{
	// Set I2S alternate function settings
	GPIO_InitTypeDef i2sPortSettings;
	GPIO_Struct_Init(&i2sPortSettings);
	i2sPortSettings.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &i2sPortSettings);
	// Now set up the alternate function.
	GPIO_PinAFConfig(GPIOA, SD_I2S_WS, SD_I2S_ALT_FUNC);
	GPIO_PinAFConfig(GPIOA, SD_I2S_CK, SD_I2S_ALT_FUNC);
	GPIO_PinAFConfig(GPIOA, SD_I2S_SD, SD_I2S_ALT_FUNC);
}

/**
 * Initializes the I2C component of the system, designed for use with the codec
 * for transmission of commands to the codec.
 */
void initI2c()
{
	// Set I2C alternate function settings
	GPIO_InitTypeDef i2cPortSettings;
	GPIO_Struct_Init(&i2cPortSettings);
	i2cPortSettings.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &i2cPortSettings);
	GPIO_PinAFConfig(GPIOB, SD_I2C_SCL, SD_I2C_ALT_FUNC);
	GPIO_PinAFConfig(GPIOB, SD_I2C_SDA, SD_I2C_ALT_FUNC);
}

void initCodec()
{

}
