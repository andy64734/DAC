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


void tlv_initI2sPins()
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

void tlv_initI2cPins()
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
