/*
 * cham_original.c
 *
 *  Created on: 2019. 3. 2.
 *      Author: Byoungjin Seok
 */

#include "cham_original.h"

/* CHAM-64/128 key schedule */
void cham_keyschedule(uint16 *k)
{
    *(k + 9) = *(k) ^ ROL(*(k ), 1) ^ ROL(*(k), 11);
    *(k + 8) = *(k + 1) ^ ROL(*(k+1), 1) ^ ROL(*(k + 1), 11);
    *(k + 11) = *(k + 2) ^ ROL(*(k+2), 1) ^ ROL(*(k + 2), 11);
    *(k + 10) = *(k + 3) ^ ROL(*(k+3), 1) ^ ROL(*(k + 3), 11);
    *(k + 13) = *(k + 4) ^ ROL(*(k+4), 1) ^ ROL(*(k + 4), 11);
    *(k + 12) = *(k + 5) ^ ROL(*(k+5), 1) ^ ROL(*(k + 5), 11);
    *(k + 15) = *(k + 6) ^ ROL(*(k+6), 1) ^ ROL(*(k + 6), 11);
    *(k + 14) = *(k + 7) ^ ROL(*(k+7), 1) ^ ROL(*(k + 7), 11);

    *(k) = *(k) ^ ROL(*(k ), 1) ^ ROL(*(k), 8);
    *(k + 1) = *(k + 1) ^ ROL(*(k+1), 1) ^ ROL(*(k + 1), 8);
    *(k + 2) = *(k + 2) ^ ROL(*(k+2), 1) ^ ROL(*(k + 2), 8);
    *(k + 3) = *(k + 3) ^ ROL(*(k+3), 1) ^ ROL(*(k + 3), 8);
    *(k + 4) = *(k + 4) ^ ROL(*(k+4), 1) ^ ROL(*(k + 4), 8);
    *(k + 5) = *(k + 5) ^ ROL(*(k+5), 1) ^ ROL(*(k + 5), 8);
    *(k + 6) = *(k + 6) ^ ROL(*(k+6), 1) ^ ROL(*(k + 6), 8);
    *(k + 7) = *(k + 7) ^ ROL(*(k+7), 1) ^ ROL(*(k + 7), 8);
}

/* CHAM-64/128 Encryption */
void cham_encryption(uint16 pt[4], uint16 *cham_rk)
{
    int i;
    uint16 tmp, X0, X1, X2, X3;

    X0 = pt[0];
    X1 = pt[1];
    X2 = pt[2];
    X3 = pt[3];

    for (i = 0; i < 80; i++)
    {
        if (i % 2 == 0)
        {
            tmp = ROL(X1, 1) ^ cham_rk[i % 16];
            tmp += (X0 ^ i);
            tmp = ROL(tmp, 8);
        }
        else
        {
            tmp = ROL(X1, 8) ^ cham_rk[i % 16];
            tmp += (X0 ^ i);
            tmp = ROL(tmp, 1);
        }

        X0 = X1;
        X1 = X2;
        X2 = X3;
        X3 = tmp;
    }

    pt[0] = X0;
    pt[1] = X1;
    pt[2] = X2;
    pt[3] = X3;
}
