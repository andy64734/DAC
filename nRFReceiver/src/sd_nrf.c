/*
 * sd_nrf.c
 *
 *  Created on: May 18, 2018
 *      Author: banikp
 */


/**
 *	Keil project for NRF24L01+ transceiver
 *
 *	Receiver code
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_nrf24l01.h"
//#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include <stdio.h>

/* Receiver address */
uint8_t TxAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
/* My address */
uint8_t MyAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};

uint8_t dataOut[32], dataIn[32];

void sd_nrf_init()
{
	TM_NRF24L01_Transmit_Status_t transmissionStatus;

	/* Initialize system */
	SystemInit();

	/* Initialize system and Delay functions */
	TM_DELAY_Init();

	/* Initialize onboard leds and button */
//	TM_DISCO_LedInit();

	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

	/* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
	/* By default 2Mbps data rate and 0dBm output power */
	/* NRF24L01 goes to RX mode by default */
	TM_NRF24L01_Init(15, 32);

	/* Set RF settings, Data rate to 2Mbps, Output power to -18dBm */
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);

	/* Set my address, 5 bytes */
	TM_NRF24L01_SetMyAddress(MyAddress);

	/* Set TX address, 5 bytes */
	TM_NRF24L01_SetTxAddress(TxAddress);
}
