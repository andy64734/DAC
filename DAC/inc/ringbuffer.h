/*
 * ringbuffer.h
 *
 *  Created on: Feb 6, 2018
 *      Author: sharana
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_
#include <inttypes.h>

// Size of the buffer
#define BUF_SIZE 50

// buffer structure
typedef struct{
	uint8_t put;
	uint8_t get;
	uint8_t empty;
	char buffer[BUF_SIZE];
} RingBuffer;

// put an element in a buffer
void put(RingBuffer*, char);

// remove an element from the buffer
char get(RingBuffer*);

// to check if the buffer is full
int hasSpace(RingBuffer*);

// to if the buffer is not empty
int hasElement(RingBuffer*);

#endif /* RINGBUFFER_H_ */
