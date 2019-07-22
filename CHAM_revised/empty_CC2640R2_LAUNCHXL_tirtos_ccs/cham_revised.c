/*
 * cham_revised.c
 *
 *  Created on: 2019. 3. 2.
 *      Author: Byoungjin Seok
 */

#include "cham_revised.h"

/* 2-way based CHAM-64/128 key schedule */
void revised_cham_keyschedule(uint32 *k)
{
    *(k  ) = (*(k  ) << 16) | *(k+2);
    *(k+1) = (*(k+1) << 16) | *(k+3);
    *(k+2) = (*(k+4) << 16) | *(k+6);
    *(k+3) = (*(k+5) << 16) | *(k+7);
    *(k+5) = T_ROL1(*(k  )) ^ T_ROL11(*(k  )) ^ *(k  );
    *(k+4) = T_ROL1(*(k+1)) ^ T_ROL11(*(k+1)) ^ *(k+1);
    *(k+7) = T_ROL1(*(k+2)) ^ T_ROL11(*(k+2)) ^ *(k+2);
    *(k+6) = T_ROL1(*(k+3)) ^ T_ROL11(*(k+3)) ^ *(k+3);
    *(k  ) = T_ROL1(*(k  )) ^ T_ROL8(*(k  )) ^ *(k  );
    *(k+1) = T_ROL1(*(k+1)) ^ T_ROL8(*(k+1)) ^ *(k+1);
    *(k+2) = T_ROL1(*(k+2)) ^ T_ROL8(*(k+2)) ^ *(k+2);
    *(k+3) = T_ROL1(*(k+3)) ^ T_ROL8(*(k+3)) ^ *(k+3);
}

/* 2-way based CHAM-64/128 encryption */
void revised_cham_encryption(uint32 *pt, uint32 *cham_rk)
{
    uint32 i = 0;
    uint32 pair_odd = 0, pair_even = 0, tmp0 = 0, tmp1 = 0;

    pair_odd  = (*(pt) & 0xffff0000) | ((*(pt+1) & 0xffff0000) >> 16);
    pair_even = ((*(pt) & 0x0000ffff) << 16) | (*(pt+1) & 0x0000ffff);

    for (i = 0; i < 20; i++)
    {
        tmp0 = T_ROL1(pair_even) ^ cham_rk[(2*i) % 8];
        tmp0 += (pair_odd ^ ((4 * i) << 16) ^ ((4 * i) + 2));
        tmp0 -= (tmp0 & 0x0000FFFF) < ((pair_odd ^ ((4 * i) << 16) ^ ((4 * i) + 2)) & 0x0000FFFF) ? 0x00010000 : 0;
        tmp0 = T_ROL8(tmp0);

        tmp1 = T_ROL8( ( (pair_odd << 16) | (tmp0 >> 16) ) ) ^ cham_rk[((2*i)+1) % 8];
        tmp1 += (pair_even ^ (((4 * i) + 1) << 16) ^ (((4 * i) + 3)));
        tmp1 -= (tmp1 & 0x0000FFFF) < ((pair_even ^ (((4 * i) + 1) << 16) ^ (((4 * i) + 3))) & 0x0000FFFF) ? 0x00010000 : 0;
        tmp1 = T_ROL1(tmp1);

        pair_odd = tmp0;
        pair_even = tmp1;
    }

    *(pt)   = (pair_odd & 0xffff0000) | ((pair_even & 0xffff0000) >> 16);
    *(pt+1) = ((pair_odd & 0x0000ffff) << 16) | (pair_even & 0x0000ffff);
}
