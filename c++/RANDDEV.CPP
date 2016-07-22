//=============================================================
//  Random Number Classes   randdev.cpp    v1.10
//      "Random number" generator that produces statistically-
//      excellent uniform deviates, modified from code
//      published in Chap. 7 of NUMERICAL RECIPES IN C, 2nd ed.
//      Copyright 1992, 1995 by Scott Robert Ladd.
//=============================================================

#include "randdev.h"
#include "float.h"

static const long   IM1 = 2147483563L;
static const long   IM2 = 2147483399L;
static const long  IMM1 = IM1 - 1L;
static const long   IA1 = 40014L;
static const long   IA2 = 40692L;
static const long   IQ1 = 53668L;
static const long   IQ2 = 52774L;
static const long   IR1 = 12211L;
static const long   IR2 =  3791L;
static const long  NTAB =    32L;
static const long  NDIV = 1L + IMM1 / long(NTAB);
static const float RNMX = 1.0F - FLT_EPSILON;
static const float   AM = 1.0F / 2147483563.0F;

float RandDev::operator () ()
    {
    long j, k;
    static long idum2 = 123456789L;
    static long iy    = 0L;
    static long iv[NTAB];
    float temp;

    if (Seed <= 0L)
        {
        if (-Seed < 1L)
            Seed = 1L;
        else
            Seed = -Seed;

        idum2 = Seed;

        for (j = NTAB + 7; j >= 0; --j)
            {
            k = Seed / IQ1;
            Seed = IA1 * (Seed - k * IQ1) - k * IR1;

            if (Seed < 0L)
                Seed += IM1;

            if (j < NTAB)
                iv[size_t(j)] = Seed;
            }

        iy = iv[0];
        }

    k = Seed / IQ1;

    Seed = IA1 * (Seed - k * IQ1) - k * IR1;

    if (Seed < 0L)
        Seed += IM1;

    k = idum2 / IQ2;

    idum2 = IA2 * (idum2 - k * IQ2) - k * IR2;

    if (idum2 < 0L)
        idum2 += IM2;

    j  = iy / NDIV;
    iy = iv[size_t(j)] - idum2;
    iv[size_t(j)] = Seed;

    if (iy < 1L)
        iy += IMM1;

    temp = AM * float(iy);

    if (temp > RNMX)
        return RNMX;
    else
        return temp;
    }
