/*
 * cham_revised.h
 *
 *  Created on: 2019. 3. 2.
 *      Author: Byoungjin Seok
 */

#ifndef CHAM_REVISED_H_
#define CHAM_REVISED_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

// 2-way ROL operation
//  -------------
//  | ROL | ROL |
//  -------------

#define T_ROL1(X)  ( ( ((X) << 1 ) & 0xFFFEFFFE ) | ( ((X) >> 15) & 0x00010001 ) )
#define T_ROL8(X)  ( ( ((X) << 8 ) & 0xFF00FF00 ) | ( ((X) >> 8 ) & 0x00FF00FF ) )
#define T_ROL11(X) ( ( ((X) << 11) & 0xF800F800 ) | ( ((X) >> 5 ) & 0x07FF07FF ) )

/* 2-way based CHAM-64/128 key schedule */
void revised_cham_keyschedule(uint32 *k);
/* 2-way based CHAM-64/128 encryption */
void revised_cham_encryption(uint32 *pt, uint32 *cham_rk);


#endif /* CHAM_REVISED_H_ */
