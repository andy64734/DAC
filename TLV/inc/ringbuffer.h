/*
 * ringbuffer.h
 *
 *  Created on: Feb 6, 2018
 *      Author: sharana
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_
#include <inttypes.h>
#include <stdbool.h>

// Size of the buffer
// The size must be strictly greater than 1, since the ring buffer
// would be detected as full if the head is directly after the tail. If
// they are at the same point, then this is detected as empty. This
// size prevents confusion.
#define BUF_SIZE 2

// buffer structure
typedef struct{
	uint8_t tail;
	uint8_t head;
	int32_t buffer[BUF_SIZE];
} RingBuffer;

void ringBufferInit(RingBuffer*);

// put an element in a buffer
bool put(RingBuffer*, int32_t);

// remove an element from the buffer
int32_t get(RingBuffer*);

// to check if the buffer is full
int hasSpace(RingBuffer*);

// to if the buffer is not empty
int hasElement(RingBuffer*);

#endif /* RINGBUFFER_H_ */
