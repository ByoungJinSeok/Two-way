/*
 * sparx_original.c
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

/* Basic functions and their inverses */
/* ================================== */

/* One keyless round of SPECK-32 */
#include "sparx_original.h"

void A(uint16 * l, uint16 * r)
{
    (*l) = ROTL((*l), 9);
    (*l) += (*r);
    (*r) = ROTL((*r), 2);
    (*r) ^= (*l);
}


/* The linear layers */

void L_2(uint16 * x)
{
    uint16 tmp = ROTL((x[0] ^ x[1]), 8);
    x[2] ^= x[0] ^ tmp;
    x[3] ^= x[1] ^ tmp;
    SWAP(x[0], x[2]);
    SWAP(x[1], x[3]);
}

/* Key schedule  */
/* ============= */

/* The permutation of the key state */
void K_perm_64_128(uint16 * k, uint16 c)
{
    uint16 tmp_0, tmp_1, i;
    /* Misty-like transformation */
    A(k+0, k+1);
    k[2] += k[0];
    k[3] += k[1];
    k[7] += c;
    /* Branch rotation */
    tmp_0 = k[6];
    tmp_1 = k[7];
    for (i=7 ; i>=2 ; i--)
    {
        k[i] = k[i-2];
    }
    k[0] = tmp_0;
    k[1] = tmp_1;
}


/* Takes a 128 bit master key and turns it into 2*(N_STEPS+1) subkeys
 * of 96 bits */
void key_schedule(uint16 subkeys[][2*ROUNDS_PER_STEPS], uint16 * master_key)
{
    uint8 c, i;
    for (c=0 ; c<(N_BRANCHES*N_STEPS+1) ; c++)
    {
        for (i=0 ; i<2*ROUNDS_PER_STEPS ; i++)
        {
            subkeys[c][i] = master_key[i];
        }
        K_perm(master_key, c+1);
    }
}


/* Encryption */
/* ========================= */

void sparx_encrypt(uint16 * x, uint16 k[][2*ROUNDS_PER_STEPS])
{
    uint8 s, r, b;

    s=0; b=0; r=0;
    for (s=0 ; s<N_STEPS ; s++)
    {
        for (b=0 ; b<N_BRANCHES ; b++)
        {
            for (r=0 ; r<ROUNDS_PER_STEPS ; r++)
            {
                x[2*b  ] ^= k[N_BRANCHES*s + b][2*r    ];
                x[2*b+1] ^= k[N_BRANCHES*s + b][2*r + 1];
                A(x + 2*b, x + 2*b+1);
            }
        }
        L(x);
    }
    for (b=0 ; b<N_BRANCHES ; b++)
    {
        x[2*b  ] ^= k[N_BRANCHES*N_STEPS][2*b  ];
        x[2*b+1] ^= k[N_BRANCHES*N_STEPS][2*b+1];
    }
}
