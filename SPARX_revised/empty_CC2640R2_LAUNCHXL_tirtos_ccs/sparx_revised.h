/*
 * sparx_revised.h
 *
 *  Created on: 2019. 3. 1.
 *      Author: Byoungjin Seok
 */

#ifndef SPARX_REVISED_H_
#define SPARX_REVISED_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

#define T_ROTR7(X) ((((X) >> 7) & 0x01FF01FF) | (((X) << 9) & 0xFE00FE00))
#define T_ROTL2(X) ((((X) << 2) & 0xFFFCFFFC) | (((X) >> 14) & 0x00030003))
#define T_ROTL8(X) ((((X) << 8) & 0xFF00FF00) | (((X) >> 8) & 0x00FF00FF))
#define ROTL(X, n) ((X) << (n)) | ((X) >> (32-(n)))
#define R_half_reset(X) X &= 0xffff0000;
#define T_ADD(X, Y) {\
    (X) += (Y);\
    (X) -= ((X) & 0x0000FFFF) < ((Y) & 0x0000FFFF) ? 0x00010000 : 0;\
}

/* 2-way based SPECKEY */
void revised_SPECKEY(uint32 * x, uint32 * y);
/* 2-way based SPARX-64/128 key permutation */
void revised_K_perm_64_128(uint32 * k, uint32 c);
/* 2-way based SPARX-64/128 key schedule */
void revised_sparx_keyschedule(uint32 * sparx_rk, uint32 * mk);
/* 2-way based SPARX-64/128 encryption */
void revised_sparx_encrypt(uint32 *pt, uint32 *sparx_rk);

#endif /* SPARX_REVISED_H_ */
