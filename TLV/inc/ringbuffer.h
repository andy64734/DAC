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
#define BUF_SIZE 1

// buffer structure
typedef struct{
	uint8_t tail;
	uint8_t head;
	uint8_t empty;
	uint16_t buffer[BUF_SIZE];
} RingBuffer;

// put an element in a buffer
bool put(RingBuffer*, uint16_t);

// remove an element from the buffer
uint16_t get(RingBuffer*);

// to check if the buffer is full
int hasSpace(RingBuffer*);

// to if the buffer is not empty
int hasElement(RingBuffer*);

#endif /* RINGBUFFER_H_ */