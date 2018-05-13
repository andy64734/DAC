/*
 * tlv.c
 *
 *  Created on: Mar 5, 2018
 *      Author: winstona
 */

#include <tlv_dac.h>
#include <tlv.h>
#include "stm32f4xx.h"

/**
 * Initializes the registers for the I2C commponent.
 */
static void _tlv_dac_initRegisters();

/**
 * Here we set method to make the proper audio settings for the TLV chip.
 */
static void _tlv_dac_I2S_audioSettings();

void tlv_dac_init()
{
	// Appropriate GPIO pins.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
			| RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); // I2C interface
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // I2S interface
	RCC_I2SCLKConfig(RCC_I2SBus_APB2, RCC_I2SCLKSource_PLLI2S);
	RCC_PLLI2SCmd(ENABLE); // Enable PLLI2S
	while (!RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY));

	tlv_initI2sPins();
	tlv_initI2cPins();
	tlv_initResetPin();

	I2C_InitTypeDef i2cSettings;
	I2C_StructInit(&i2cSettings);

	// Initialize the I2C radio with the appropriate settings.
	// The default settings in i2cSettings should be fine.
	I2C_Init(SD_I2C_INTERFACE, &i2cSettings);
	_tlv_dac_initRegisters();
	_tlv_dac_I2S_audioSettings();
}

/*
 * To initialize I2S for the TLV chip, by setting the correct audio settings.
 */
static void _tlv_dac_I2S_audioSettings()
{
	I2S_InitTypeDef I2S_InitStructure;

	SPI_I2S_DeInit(SD_I2S_INTERFACE);
	// Audio is recorded from two channels
	I2S_InitStructure.I2S_AudioFreq = 44100 * 2;
	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16bextended;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_Init(SD_I2S_INTERFACE, &I2S_InitStructure);
	I2S_Cmd(SD_I2S_INTERFACE, ENABLE);

}
static void _tlv_dac_initRegisters()
{
	// Any convention of not using read-modify-write here is because
	// registe values are by default, and that if not, we would want
	// these register values to be reset anyway.

	// Set the page to 0. 'Tis absolutely vital.
	tlv_i2c_write(TLV_PAGE_SELECT_REG, 0);
	// Set the data paths, and sampling rate to 44100.
	tlv_i2c_write(TLV_DATA_PATH_REG, (1 << 7) |  (TLV_LEFT_IN << TLV_LEFT_PATH)
			| (TLV_RIGHT_IN << TLV_RIGHT_PATH));
	// Set the word length to 16 bits, and I2S communication.
	tlv_i2c_write(TLV_SDI_CR_B, (TLV_I2S_IF << TLV_SD_TRANSFER)
			| (0x00 << TLV_SD_WORD_LEN));
	// Register 37 will allow for powering up of DACs. Use remaining defaults.
	tlv_i2c_write(TLV_DAC_POWER, (1 << TLV_LEFT_DAC_PWR)
			| (1 << TLV_RIGHT_DAC_PWR));
	// TODO Change the route so it goes to the high power output.
	// Route DAC_L3 signal to to left line output driver, and DAC_R3 to
	// right line output driver. This disables an unnecessary volume
	// control, and involves fewer writes. A potentiometer can be used
	// for this instead. The advantage is fewer register writes.
	tlv_i2c_write(TLV_DAC_OUT_SWITCH, (2 << 6) | (2 << 4));
	// Unmute the left DAC.
	tlv_i2c_write(TLV_LEFT_DAC_VOL, 0 << 7);
	// Do the same in the right DAC.
	tlv_i2c_write(TLV_RIGHT_DAC_VOL, 0 << 7);
	// Turn on the high power registers, and unmutes them.
	tlv_i2c_write(TLV_HPLOUT_OUT_LEVEL, (1 << 3) | (1 << 2) | (1 << 1) | 1);
	tlv_i2c_write(TLV_HPLCOM_OUT_LEVEL, (1 << 3) | (1 << 2) | (1 << 1) | 1);
	char powerStatus = tlv_i2c_read(TLV_PWR_STATUS);
	tlv_i2c_write(TLV_HPROUT_OUT_LEVEL, (1 << 3) | (1 << 2) | (1 << 1) | 1);
	tlv_i2c_write(TLV_HPRCOM_OUT_LEVEL, (1 << 3) | (1 << 2) | (1 << 1) | 1);
	powerStatus = tlv_i2c_read(TLV_PWR_STATUS);
	// Set the clock source to the CLKDIV. Does not divide by default.
	tlv_i2c_write(TLV_CLOCK, 1);

}
