/*
 * tlv.h
 *
 *  Created on: Mar 5, 2018
 *      Author: winstona
 */

#ifndef TLV_H_
#define TLV_H_

#include <inttypes.h>
#include "stm32f4xx.h"
#include "ringbuffer.h"

/******************************************************************************
 * GPIO Pin Settings
 *****************************************************************************/

#define TLV_RESET_PORT			(GPIOB)
#define TLV_RESET_PIN			(10)

#define SD_I2S_ALT_FUNC 		(GPIO_AF_SPI1)
#define SD_I2C_ALT_FUNC 		(GPIO_AF_I2C1)

#define SD_I2C_INTERFACE 		(I2C1)
#define SD_I2S_INTERFACE 		(SPI1)

#define SD_I2S_PORT				(GPIOA)
#define SD_I2S_MCLK_PORT		(GPIOC)

#define SD_I2S_WS				(4)
#define SD_I2S_CK				(5)
#define SD_I2S_SD				(7)
#define SD_I2S_MCLK				(4)

#define SD_I2S_WS_PIN			(GPIO_Pin_4)
#define SD_I2S_CK_PIN			(GPIO_Pin_5)
#define SD_I2S_SD_PIN			(GPIO_Pin_7)
#define SD_I2S_MCLK_PIN			(GPIO_Pin_4)

#define SD_I2C_PORT				(GPIOB)

#define SD_I2C_SCL 				(8)
#define SD_I2C_SDA				(9)

#define SD_I2C_SCL_PIN			(GPIO_Pin_8)
#define SD_I2C_SDA_PIN			(GPIO_Pin_9)

/******************************************************************************
 * TLV Register Names and Structs
 *****************************************************************************/

// As a note, all registers in this API will follow the convention of using an
// 8 bit value. The less 7 significant bits will indicate the page, and the
// most significant one indicates whether to use bank 0 or 1.
// The below registers are only available on bank 0.
// Page Select register
#define TLV_PAGE_SELECT_REG		(0)
// Codec data-path setup register
#define TLV_DATA_PATH_REG		(0x7)
// Serial data interface control register B
#define TLV_SDI_CR_B			(0x9)
// Left-ADC PGA Gain Control Register
#define TLV_LEFT_ADC_PGA_GAIN	(15)
// Right-ADC PGA Gain Control Register
#define TLV_RIGHT_ADC_PGA_GAIN	(16)
// MIC1LP/LINE1LP to Left-ADC Control Register
#define TLV_MIC1LP_LEFT_ADC		(19)
// MIC1RP/LINE1RP to Right-ADC Control Register
#define TLV_MIC1RP_RIGHT_ADC	(22)
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
// HPLCOM Output Level Control Register
#define TLV_HPLCOM_OUT_LEVEL	(58)
// HPROUT Output Level Control Register
#define TLV_HPROUT_OUT_LEVEL	(65)
// HPRCOM Output Level Control Register
#define TLV_HPRCOM_OUT_LEVEL	(72)
// Module Power Status Register
#define TLV_PWR_STATUS			(94)
// Clock Register
#define TLV_CLOCK				(101)

#define TLV_REG_MASK			(0x7F)

// TODO Determine whether the address should be offset to make way
// for the read/write bit. This may involve some documentation
// research.
#define TLV_I2C_ADDR			(0x30) // 7 bit address shifted left.

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

/******************************************************************************
 * Function Declarations
 *****************************************************************************/


/**
 * Initializes the I2S component of the system, designed for use with the codec
 * for transmission of audio data.
 */
void tlv_initI2sPins();

/**
 * Initializes the I2C component of the system, designed for use with the codec
 * for transmission of commands to the codec.
 */
void tlv_initI2cPins();

/**
 * Sets up the TLV chip's reset pin, and then uses it to reset the TLV codec.
 */
void tlv_initResetPin();

/**
 * Here we have a method to write data to an I2S port from a ring buffer.
 * This is non-blocking, and returns a bool of whether we're successful or not.
 */
bool SD_I2S_write(RingBuffer* I2S_buffer);

/**
 * Here we have a method to read data into a ring buffer from a I2S port.
 */
void SD_I2S_read(SPI_TypeDef* SPIx, RingBuffer* I2S_buffer);

/**
 * Enables all required interfaces for the functioning of the
 * TLV codec chip in DAC mode. This include the I2C1 interface on
 * GPIOB, and the I2S1 interface on GPIOA.
 */
void tlv_dac_init();

/**
 * Write a value to an address of a register.
 * Parameters: address - The address to write to.
 * 				message - a byte message for what to write.
 */
void tlv_i2c_write(uint8_t address, uint8_t message);

/**
 * Reads an I2C value from the register.
 */
uint8_t tlv_i2c_read(uint8_t address);

#endif /* TLV_H_ */
