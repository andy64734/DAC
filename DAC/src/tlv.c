/*
 * tlv.c
 *
 *  Created on: Mar 5, 2018
 *      Author: winstona
 */

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
 * Initializes the I2S component of the system, designed for use with the codec
 * for transmission of audio data.
 */
static void _tlv_initI2sPins();

/**
 * Initializes the I2C component of the system, designed for use with the codec
 * for transmission of commands to the codec.
 */
static void _tlv_initI2cPins();

/**
 *
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

	_tlv_initI2sPins();
	_tlv_initI2cPins();
	_tlv_dac_initRegisters();
}

static void _tlv_initI2sPins()
{
	// Set I2S alternate function settings
	GPIO_InitTypeDef i2sPortSettings;
	GPIO_StructInit(&i2sPortSettings);
	i2sPortSettings.GPIO_Mode = GPIO_Mode_AF;
	i2sPortSettings.GPIO_Pin = SD_I2S_WS_PIN | SD_I2S_CK_PIN | SD_I2S_SD_PIN;
	GPIO_Init(GPIOA, &i2sPortSettings);
	// Now set up the alternate function for each the pins.
	GPIO_PinAFConfig(GPIOA, SD_I2S_WS, SD_I2S_ALT_FUNC);
	GPIO_PinAFConfig(GPIOA, SD_I2S_CK, SD_I2S_ALT_FUNC);
	GPIO_PinAFConfig(GPIOA, SD_I2S_SD, SD_I2S_ALT_FUNC);
}

static void _tlv_initI2cPins()
{
	// Set I2C alternate function settings
	GPIO_InitTypeDef i2cPortSettings;
	GPIO_StructInit(&i2cPortSettings);
	i2cPortSettings.GPIO_Pin = SD_I2C_SCL_PIN | SD_I2C_SDA_PIN;
	i2cPortSettings.GPIO_Mode = GPIO_Mode_AF;
	// I2C standard requires pull-up resistors to avoid bus contention.
	// However, external, strong 2.2 k-ohm resistors will help us out.
	i2cPortSettings.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &i2cPortSettings);
	GPIO_PinAFConfig(GPIOB, SD_I2C_SCL, SD_I2C_ALT_FUNC);
	GPIO_PinAFConfig(GPIOB, SD_I2C_SDA, SD_I2C_ALT_FUNC);

	I2C_InitTypeDef i2cInterfaceSettings;
	I2C_StructInit(&i2cInterfaceSettings);
}

static void _tlv_dac_initRegisters()
{
	// Any convention of not using read-modify-write here is because
	// registe values are by default, and that if not, we would want
	// these register values to be reset anyway.

	// Set the data paths.
	tlv_dac_write_reg(TLV_DATA_PATH_REG, (TLV_LEFT_IN << TLV_LEFT_PATH)
			| (TLV_RIGHT_IN << TLV_RIGHT_PATH));
	// Set the word length to 16 bits, and I2S communication.
	tlv_dac_write_reg(TLV_SDI_CR_B, (TLV_I2S_IF << TLV_SD_TRANSFER)
			| (0x00 << TLV_SD_WORD_LEN));
	// Register 37 will allow for powering up of DACs. Use remaining defaults.
	tlv_dac_write_reg(TLV_DAC_POWER, (1 << TLV_LEFT_DAC_PWR)
			| (1 << TLV_RIGHT_DAC_PWR));
	// TODO Change the route so it goes to the high power output.
	// Route DAC_L3 signal to to left line output driver, and DAC_R3 to
	// right line output driver. This disables an unnecessary volume
	// control, and involves fewer writes. A potentiometer can be used
	// for this instead. The advantage is fewer register writes.
	tlv_dac_write_reg(TLV_DAC_OUT_SWITCH, (2 << 6) | (2 << 4));
	// Unmute the left DAC.
	tlv_dac_write_reg(TLV_LEFT_DAC_VOL, 1 << 7);
	// Do the same in the right DAC.
	tlv_dac_write_reg(TLV_RIGHT_DAC_VOL, 1 << 7);

}

void tlv_i2c_write(uint8_t address, uint8_t message)
{
	// Wait for I2C interface to become available. Probably won't be a problem,
	// but just in case.
	while (I2C_GetFlagStatus(SD_I2C_INTERFACE, I2C_FLAG_BUSY));
	I2C_GenerateSTART(SD_I2C_INTERFACE, ENABLE);
	I2C_Send7bitAddress(SD_I2C_INTERFACE, address, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(SD_I2C_INTERFACE,
				I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	// Now send an address.
	I2C_SendData(SD_I2C_INTERFACE, address);
	while (!I2C_CheckEvent(SD_I2C_INTERFACE,
			I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	// And finally send the actual data.
	I2C_SendData(SD_I2C_INTERFACE, message);
	while (!I2C_CheckEvent(SD_I2C_INTERFACE,
			I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	// TODO Delay here, or polling?
	I2C_GenerateSTOP(SD_I2C_INTERFACE, ENABLE);
	// START doesn't need to be disabled, since the start bit generation
	// is disabled after the start bit has been successfully sent.
}

uint8_t tlv_i2c_read(uint8_t address)
{
	while (I2C_GetFlagStatus(SD_I2C_INTERFACE, I2C_FLAG_BUSY));
	I2C_GenerateSTART(SD_I2C_INTERFACE, ENABLE);
	I2C_Send7bitAddress(SD_I2C_INTERFACE, address, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(SD_I2C_INTERFACE,
				I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	// Send an address, and wait to receive a byte back.
	I2C_SendData(SD_I2C_INTERFACE, address);
	while (!I2C_CheckEvent(SD_I2C_INTERFACE,
			I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	// STOP generated after receiving next byte.
	I2C_GenerateSTOP(SD_I2C_INTERFACE, ENABLE);
	while (!I2C_CheckEvent(SD_I2C_INTERFACE, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return I2C_ReceiveData(SD_I2C_INTERFACE);
}
