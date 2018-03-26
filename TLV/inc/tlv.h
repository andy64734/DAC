/*
 * tlv.h
 *
 *  Created on: Mar 5, 2018
 *      Author: winstona
 */

#ifndef TLV_H_
#define TLV_H_

#include <inttypes.h>

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
 *
 */

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
 *
 */
uint8_t tlv_i2c_read(uint8_t address);

#endif /* TLV_H_ */
