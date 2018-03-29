/*
 * tlv_adc.c
 *
 *  Created on: Mar 26, 2018
 *      Author: winstona
 */

#include "tlv_adc.h"
#include "stm32f4xx.h"

static void _tlv_adc_I2S_audioSettings();

/**
 * Initializes the registers on the TLV chip in preparation for
 * use as an ADC. Will initialize through the I2C interface.
 */
static void _tlv_adc_initRegisters();

void tlv_adc_init()
{
	// Appropriate GPIO pins.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); // I2C interface
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // I2S interface

	// Prepares the settings for the I2C radio.
	I2C_InitTypeDef i2cSettings;
	I2C_StructInit(&i2cSettings);

	// Initialize the I2C radio with the appropriate settings.
	// The default settings in i2cSettings should be fine.
	I2C_Init(SD_I2C_INTERFACE, &i2cSettings);

	tlv_initI2sPins();
	tlv_initI2cPins();
	_tlv_adc_initRegisters();
	_tlv_adc_I2S_audioSettings();
}

static void _tlv_adc_I2S_audioSettings()
{

}

static void _tlv_adc_initRegisters()
{
}