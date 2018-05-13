/*
 * ringbuffer.c
 *
 *  Created on: Feb 6, 2018
 *      Author: sharana
 *      Description: Program to initiate a ring buffer.
 */
#include "ringbuffer.h"
#include <inttypes.h>
#include <stdbool.h>

void ringBufferInit(RingBuffer* buffer)
{
	buffer->head = 0;
	buffer->tail = 0;
}
/*
 * Here we have a method put characters into the buffer.
 * @param buffer it is a structure type to access the correct ring buffer.
 * @param element it is the character.
 */
bool put(RingBuffer* buffer, int32_t element)
{
	//Check space
	if (hasSpace(buffer) != 0)
	{	//adding element to the buffer
		buffer->buffer[buffer->tail] = element;
		buffer->tail = (buffer->tail + 1) % BUF_SIZE;
		return true;
	}
	else
	{
		// drop the pact
		return false;
	}
}
/*
 * Here we have a method to get and remove characters into the buffer.
 * @param buffer it is a structure type to access the correct ring buffer.
 */
int32_t get(RingBuffer* buffer)
{
	//check space
	if (hasElement(buffer) != 0)
	{	//get character from the  buffer
		int32_t element = buffer->buffer[buffer->head];
		//adjusting get
		buffer->head = (buffer->head+1) % BUF_SIZE;
		return element;
	}
	return '\0';
}
/*
 * Returns true (non-zero) if there is room for one element in buffer
 * @param buffer it is a structure type to access the correct ring buffer.
 */
int hasSpace(RingBuffer* buffer)
{
	if ((((buffer->tail + 1) % BUF_SIZE)) == buffer->head)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
 * return true (non-zero) if there is at least one element in buffer
 * @param buffer it is a structure type to access the correct ring buffer.
 */
int hasElement(RingBuffer * buffer){
	if (buffer->head==buffer->tail)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
