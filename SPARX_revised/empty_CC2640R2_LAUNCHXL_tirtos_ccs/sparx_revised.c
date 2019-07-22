#include "sparx_revised.h"

/* 2-way based SPECKEY */
void revised_SPECKEY(uint32 * x, uint32 * y)
{
    (*x) = T_ROTR7((*x));
    T_ADD((*x), (*y));
    (*y) = T_ROTL2((*y)) ^ (*x);
}

/* 2-way based SPARX-64/128 key permutation */
void revised_K_perm_64_128(uint32 * k, uint32 c)
{
    uint32 tmp[2], i;

    for (i = 0; i < 8; i++)
    {
        *(k + i) <<= 16;
    }

    *(k) |= (*(k + 6) >> 16);
    *(k + 1) |= (*(k + 7) >> 16);
    *(k + 6) |= (*(k + 4) >> 16);
    *(k + 7) |= (*(k + 5) >> 16);

    T_ADD(*(k + 7), ((c + 1) << 16) | (c + 2));
    T_ADD(*(k + 1), (c + 1));

    revised_SPECKEY((k + 0), (k + 1));

    *(k + 2) |= (*(k) >> 16);
    *(k + 3) |= (*(k + 1) >> 16);

    T_ADD(*(k + 2), *(k));
    T_ADD(*(k + 3), *(k + 1));

    *(k + 4) |= (*(k + 2) >> 16);
    *(k + 5) |= (*(k + 3) >> 16);

    *(tmp) = *(k + 6);
    *(tmp + 1) = *(k + 7);
    *(k + 6) = *(k + 4);
    *(k + 7) = *(k + 5);
    *(k + 4) = *(k + 2);
    *(k + 5) = *(k + 3);
    *(k + 2) = *(k);
    *(k + 3) = *(k + 1);
    *(k) = *(tmp);
    *(k + 1) = *(tmp + 1);
}

/* 2-way based SPARX-64/128 key schedule */
void revised_sparx_keyschedule(uint32 * sparx_rk, uint32 * mk)
{
    int i, j;
    uint32 tmp[8];

    /* First key update */
    for (i = 0; i < 8; i++)
    {
        *(tmp + i) = *(mk + i) << 16;
    }
    *(mk + 7) += 1;
    revised_SPECKEY(mk, mk + 1);
    T_ADD(*(mk + 2), *(mk));
    T_ADD(*(mk + 3), *(mk + 1));
    *(tmp) += *(mk + 6);
    *(tmp + 1) += *(mk + 7);
    *(mk + 6) = *(tmp + 6) + *(mk + 4);
    *(mk + 7) = *(tmp + 7) + *(mk + 5);
    *(mk + 4) = *(tmp + 4) + *(mk + 2);
    *(mk + 5) = *(tmp + 5) + *(mk + 3);
    *(mk + 2) = *(tmp + 2) + *(mk);
    *(mk + 3) = *(tmp + 3) + *(mk + 1);
    *(mk) = *(tmp);
    *(mk + 1) = *(tmp + 1);
    for (j = 0; j < 6; j++)
    {
        *(sparx_rk + j) = *(mk + j);
    }

    // Revised key updating
    for (i = 0; i < 8; i++)
    {
        // Key update
        revised_K_perm_64_128(mk, (2 * i) + 1);
        for (j = 0; j < 6; j++)
        {
            *(sparx_rk + (6 * (i + 1)) + j) = *(mk + j);
        }
    }
}

/* 2-way based SPARX-64/128 encryption */
void revised_sparx_encrypt(uint32 *pt, uint32 *sparx_rk)
{
    int i;
    uint32 tmp[8];

    /* Align a plaintext */
    *(tmp) = *(pt);
    *(pt) = (*(pt) & 0xffff0000) | ((*(pt + 1) & 0xffff0000) >> 16);
    *(pt + 1) = (*(pt + 1) & 0x0000ffff) | ((*(tmp) & 0x0000ffff) << 16);

    for (i = 0; i < 8; i++)
    {
        // Round function
        /* Round function - A^3 */
        *(pt) ^= *(sparx_rk + (6 * i));
        *(pt + 1) ^= *(sparx_rk + (6 * i) + 1);
        revised_SPECKEY(pt, pt + 1);
        *(pt) ^= *(sparx_rk + (6 * i) + 2);
        *(pt + 1) ^= *(sparx_rk + (6 * i) + 3);
        revised_SPECKEY(pt, pt + 1);
        *(pt) ^= *(sparx_rk + (6 * i) + 4);
        *(pt + 1) ^= *(sparx_rk + (6 * i) + 5);
        revised_SPECKEY(pt, pt + 1);
        /* Round function - L */
        *(tmp) = *(pt) ^ *(pt + 1);
        *(tmp) = (T_ROTL8((*tmp))) >> 16;
        *(pt) ^= (*(tmp) ^ (*(pt) >> 16));
        *(pt + 1) ^= (*(tmp) ^ (*(pt + 1) >> 16));
        *(pt) = ROTL(*pt, 16);
        *(pt + 1) = ROTL(*(pt + 1), 16);
    }

    /* Align a ciphertext and last key xor */
    *(tmp) = *(pt);
    *(pt) = ((*(pt) & 0xffff0000) | ((*(pt + 1) & 0xffff0000) >> 16))
            ^ (*(sparx_rk + (6 * i)) & 0xffff0000)
            ^ ((*(sparx_rk + (6 * i) + 1) & 0xffff0000) >> 16);
    *(pt + 1) = ((*(pt + 1) & 0x0000ffff) | ((*(tmp) & 0x0000ffff) << 16))
            ^ (*(sparx_rk + (6 * i) + 2) & 0xffff0000)
            ^ ((*(sparx_rk + (6 * i) + 3) & 0xffff0000) >> 16);
}
