#include <inttypes.h>
#ifndef DELAY_H_DEF
#define DELAY_H_DEF

//Registers for STK
#define STK_CTRL (volatile uint32_t*) 0xE000E010
#define STK_LOAD (volatile uint32_t*) 0xE000E014
#define STK_VAL (volatile uint32_t*) 0xE000E018
#define STK_ENABLE_F 0
#define STK_CLKSOURCE_F 2
#define STK_CNTFLAG_F 16
#endif /*DELAY_H_DEF*/

extern void delay_us(int);

extern void delay_ms(int);
