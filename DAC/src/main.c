/**
  ******************************************************************************
  * @file    main.c
  * @author  Team Mu
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include <tlv.h>
#include <inttypes.h>
#include "ringbuffer.h"
#include "stm32f4xx.h"
#include "math.h"

#define WAVE_FREQ (1000)
#define SAMP_RATE (44100.0)

int main(void)
{
	tlv_dac_init();
	// Create and initialize a ring buffer.
	RingBuffer theBuffer;
	ringBufferInit(&theBuffer);
	unsigned currTime;
	for(;;)
	{
		if (hasSpace(&theBuffer))
		{
			// Create the new value in the buffer.
			double waveValue = sin(2 * M_PI * WAVE_FREQ * currTime / SAMP_RATE);
			int16_t regValue = waveValue * pow(2,15);
			put(&theBuffer, regValue);
			currTime++;
		}
	}
}
