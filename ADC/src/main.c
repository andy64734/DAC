/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "tlv_adc.h"
#include "stm32f4xx.h"
			

int main(void)
{
	tlv_adc_init();
	for(;;);
}
