/*
 * tlv.c
 *
 *  Created on: Mar 5, 2018
 *      Author: winstona
 */

#include <tlv_dac.h>
#include <tlv.h>
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

#define SD_I2S_ALT_FUNC 		(GPIO_AF_SPI1)
#define SD_I2C_ALT_FUNC 		(GPIO_AF_I2C1)

#define SD_I2C_INTERFACE 		I2C1
#define SD_I2S_INTERFACE 		SPI1

#define SD_I2S_WS				(4)
#define SD_I2S_CK				(5)
#define SD_I2S_SD				(6)

#define SD_I2S_WS_PIN			(GPIO_Pin_4)
#define SD_I2S_CK_PIN			(GPIO_Pin_5)
#define SD_I2S_SD_PIN			(GPIO_Pin_6)

#define SD_I2C_SCL 				(8)
#define SD_I2C_SDA				(9)

#define SD_I2C_SCL_PIN			(GPIO_Pin_8)
#define SD_I2C_SDA_PIN			(GPIO_Pin_9)

// As a note, all registers in this API will follow the convention of using an
// 8 bit value. The less 7 significant bits will indicate the page, and the
// most significant one indicates whether to use bank 0 or 1.
// Codec data-path setup register
#define TLV_DATA_PATH_REG		(0x7)
// Serial data interface control register B
#define TLV_SDI_CR_B			(0x9)
// DAC Power and Output Driver Control Register
#define TLV_DAC_POWER			(37)
// DAC Output Switching Control Register
#define TLV_DAC_OUT_SWITCH		(41)
// Left DAC Digital Volume Control Register
#define TLV_LEFT_DAC_VOL		(43)
// Right DAC Digital Volume Control Register
#define TLV_RIGHT_DAC_VOL		(44)
// HPLOUT Output Level Control Register
#define TLV_HPLOUT_OUT_LEVEL	(51)
// HPROUT Output Level Control Register
#define TLV_HPROUT_OUT_LEVEL	(65)

#define TLV_REG_MASK			(0x7F)

#define TLV_I2C_ADDR			(0x18) // 7 bit address

// Offsets for data paths in data path register.
#define TLV_LEFT_PATH			(0x3)
#define TLV_RIGHT_PATH			(0x1)
// The kinds of data that can go in each path.
typedef enum {TLV_NO_IN = 0, TLV_LEFT_IN = 1,
TLV_RIGHT_IN = 2, TLV_MONO_IN = 3} _TLV_INPUT;

// Bit positions in TLV_SDI_CR_B
#define TLV_SD_TRANSFER			(0x6)
#define TLV_SD_WORD_LEN			(0x4)
// Different kinds of interfaces
typedef enum {TLV_I2S_IF = 0, TLV_DSP_IF = 1,
TLV_LEFT_IF = 2, TLV_RIGHT_IF = 3} _TLV_INTERFACE;

#define TLV_LEFT_DAC_PWR		(0x7)
#define TLV_RIGHT_DAC_PWR		(0x6)

/**
 *
 */
static void _tlv_dac_I2S_audioSettings();

/**
 * Initializes the registers for the I2C commponent.
 */
static void _tlv_dac_initRegisters();

void tlv_dac_init()
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
	_tlv_dac_initRegisters();
}

static void _tlv_dac_I2S_audioSettings()
{
	I2S_InitTypeDef I2S_InitStructure;

	SPI_I2S_DeInit(SPI2);
	I2S_InitStructure.I2S_AudioFreq = 44100 * 2; // Audio is recorded from two channels, so this value must be twice the value in the PDM filter
	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_Init(SPI1, &I2S_InitStructure);

}

static void _tlv_dac_initRegisters()
{
	// Any convention of not using read-modify-write here is because
	// registe values are by default, and that if not, we would want
	// these register values to be reset anyway.

	// Set the data paths.
	tlv_i2c_write(TLV_DATA_PATH_REG, (TLV_LEFT_IN << TLV_LEFT_PATH)
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
	tlv_i2c_write(TLV_LEFT_DAC_VOL, 1 << 7);
	// Do the same in the right DAC.
	tlv_i2c_write(TLV_RIGHT_DAC_VOL, 1 << 7);

}
