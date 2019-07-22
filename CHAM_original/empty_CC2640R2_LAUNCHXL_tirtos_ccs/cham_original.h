/*
 * cham_original.h
 *
 *  Created on: 2019. 3. 2.
 *      Author: Byoungjin Seok
 */

#ifndef CHAM_ORIGINAL_H_
#define CHAM_ORIGINAL_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define ROL(X, n) ((((X) << (n)) | ((X) >> (16 - (n)))))

/* CHAM-64/128 key schedule */
void cham_keyschedule(uint16 *k);
/* CHAM-64/128 Encryption */
void cham_encryption(uint16 pt[4], uint16 *cham_rk);

#endif /* CHAM_ORIGINAL_H_ */
