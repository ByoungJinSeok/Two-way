/*
 * sparx_original.h
 *
 *  Modified on: 2019. 3. 1.
 *      Author: Byoungjin Seok
 */

/*
 * The original source code is located in:
 *
 * SPARX source code package
 *
 * Copyright (C) 2016, 2017 CryptoLUX (https://www.cryptolux.org)
 *
 * Written by Leo Perrin <leo.perrin@uni.lu> and
              Daniel Dinu <dumitru-daniel.dinu@uni.lu>
 *
 * You may use this work under the terms of a Creative Commons CC0 1.0
 * License/Waiver or the Apache Public License 2.0, at your option. The terms of
 * these licenses can be found at:
 *
 * - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
 * - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0
 *
 * You should have received a copy of both of these licenses along with this
 * software. If not, they may be obtained at the above URLs.
 */

#ifndef SPARX_ORIGINAL_H_
#define SPARX_ORIGINAL_H_

typedef signed char int8;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

/* SPARX instances */
#define SPARX_64_128  0

/*
 * Select SPARX instance from:
 *  - SPARX_64_128
 */
#ifndef SPARX_INSTANCE
#define SPARX_INSTANCE SPARX_64_128
#endif


#define ROTL(x, n) (((x) << n) | ((x) >> (16 - (n))))
#define SWAP(x, y) tmp = x; x = y; y = tmp


#if (SPARX_INSTANCE == SPARX_64_128)

#define N_STEPS 8
#define ROUNDS_PER_STEPS 3
#define N_BRANCHES 2
#define K_SIZE 4
#define L L_2
#define L_inv L_2_inv
#define K_perm K_perm_64_128

#endif

void A(uint16 * l, uint16 * r);
void L_2(uint16 * x);
void K_perm_64_128(uint16 * k, uint16 c);
void key_schedule(uint16 subkeys[][2*ROUNDS_PER_STEPS], uint16 * master_key);
void sparx_encrypt(uint16 * x, uint16 k[][2*ROUNDS_PER_STEPS]);

#endif /* SPARX_ORIGINAL_H_ */
