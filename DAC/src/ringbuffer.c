/*
 * ringbuffer.c
 *
 *  Created on: Feb 6, 2018
 *      Author: sharana
 *      Description: Program to initiate a ring buffer.
 */
#include"ringbuffer.h"
#include<inttypes.h>
/*
 * Here we have a method put characters into the buffer.
 * @param buffer it is a structure type to access the correct ring buffer.
 * @param element it is the character.
 */
void put(RingBuffer* buffer, char element){
	//Check space
	 if(hasSpace(&buffer)== 1)
	    {	//adding element to the buffer
	        buffer->buffer[buffer->put] = element;
	        buffer->put = (buffer->put + 1) % BUF_SIZE;
	    }else{
	    	//it it is at the get
	            buffer->get = (buffer->get + 1) % BUF_SIZE;
	            buffer->buffer[buffer->get] = element;

	        }
	    }
/*
 * Here we have a method to get and remove characters into the buffer.
 * @param buffer it is a structure type to access the correct ring buffer.
 */
char get(RingBuffer* buffer){
	//check space
	if(hasElement(&buffer)== 1){
		//get character from the  buffer
	char element = buffer->buffer[buffer->get];
	//adjusting get
	buffer->get = (buffer->get+1) % BUF_SIZE;
	return element;

	}
}
/*
 * Returns true (non-zero) if there is room for one element in buffer
 * @param buffer it is a structure type to access the correct ring buffer.
 */
int hasSpace(RingBuffer * buffer){
	if((((buffer->put + 1) % BUF_SIZE)) == buffer->get){
		return 0;
	}else{
		return 1;
	}
}

/*
 * return true (non-zero) if there is at least one element in buffer
 * @param buffer it is a structure type to access the correct ring buffer.
 */
int hasElement(RingBuffer * buffer){
	if(buffer->get==buffer->put){
			return 0;
		}else{
			return 1;
		}
}
