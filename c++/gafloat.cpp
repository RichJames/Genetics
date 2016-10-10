//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      gafloat.cpp     v1.00
//
//      Defines operations for performing mutation and
//      one-point crossover on floating-point values.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include <iostream>
#include <cmath>
#include "gafloat.h"
#include "float.h"
#include "randdev.h"
#include "string.h"

using namespace std;

static RandDev devgen;

FloatMutagen::FloatMutagen
    (
    const float & sweight,
    const float & eweight,
    const float & mweight
    )
    : TotalW(sweight + eweight + mweight),
      SignW(sweight),
      ExpW(eweight)
    {
    // intentionally blank
    }

float FloatMutagen::MPick()
    {
	return float(devgen() * TotalW);
    }

float FloatMutagen::Mutate
    (
    const float & f
    )
    {
    // mask for exponent bits
    static const long FExpt = 0x7F800000L;

    long x, n, mask;

    // choose section to mutate
    float mpick = devgen() * TotalW;

    // copy float to long for manipulation
    memcpy(&x,&f,sizeof(long));

    // if all exponent bits on (invalid #), return original
    if ((x & FExpt) == FExpt)
        return f;

    // mutate
    if (mpick < SignW)
        {
        // flip sign
        mask = 0x80000000L;

        if (x & mask)
            x &= ~mask;
        else
            x |= mask;
        }
    else
        {
        mpick -= SignW;

        if (mpick < ExpW)
            {
            // mutate exponent while number is valid
            do  {
                n    = x;
                mask = 0x00800000L << int(devgen() * 8.0F);

                if (n & mask)
                    n &= ~mask;
                else
                    n |= mask;
                }
            while ((n & FExpt) == FExpt);

            x = n;
            }
        else
            {
            // flip bit in mantissa
            mask = 1L << int(devgen() * 23.0F);

            if (x & mask)
                x &= ~mask;
            else
                x |= mask;
            }
        }

    // done!
    float res;
    memcpy(&res,&x,sizeof(float));
    return res;
    }

#ifdef __BORLANDC__
#pragma argsused
#endif

double FloatMutagen::Mutate
    (
    const double & d
    )
    {
	//cout << "in FloatMutagen::Mutate" << endl;
    // mask for exponent bits
	double dcopy = d;
    static const long DExpt = 0x7FF00000UL;

    long x[2], n, mask, bit;

    // choose section to mutate
    double mpick = devgen() * TotalW;
    //double mpick = 0.05648113591638233 * TotalW;
    // double mpick = 37.0747;
	//cout << "mpick = " << mpick << endl;

    // copy double to pair of longs for manipulation
    memcpy(x,&d,2 * sizeof(long));

    if (mpick < SignW)
        {
        // flip sign
        mask = 0x80000000L;

        if (x[1] & mask)
            x[1] &= ~mask;
        else
            x[1] |= mask;
        }
    else
        {
        mpick -= SignW;

        if (mpick < ExpW)
            {
            // mutate exponent while number is valid
            do  {
                n = x[1];
                mask = 0x00100000L << int(devgen() * 11.0F);
                //mask = 0x00100000L << int(94.0542 * 11.0F);
		//mask = 1073741824;

                if (n & mask)
                    n &= ~mask;
                else
                    n |= mask;
                }
            while ((n & DExpt) == DExpt);

            x[1] = n;
            }
        else
            {
            bit = long(devgen() * 52.0F);
		//cout << "bit = " << bit << endl;
            //bit = long(23.5392 * 52.0F);

            if (bit > 31L)
                {
                bit -= 32L;
                mask = 1L << (int)bit;
		//cout << "mask = " << mask << ", x[1] = " << x[1] << endl;

                if (x[1] & mask)
                    x[1] &= ~mask;
                else
                    x[1] |= mask;
                }
            else
                {
                // flip bit in mantissa
                mask = 1L << (int)bit;
		//cout << "mask = " << mask << ", x[0] = " << x[0] << endl;

                if (x[0] & mask)
                    x[0] &= ~mask;
                else
                    x[0] |= mask;
                }
            }
        }

    // done
    double res;
    memcpy(&res,x,sizeof(double));
	if (std::abs(res) < 1.0E-300)
		cout << "Mutate:  res is too small - " << res << ", d was " << dcopy << endl;
    return res;
    }

// crossover
float Crossover
    (
    const float & f1,
    const float & f2
    )
    {
    // mask for exponent bits
    static const long FExpt = 0x7F800000L;

    long  l1, l2, lcross, mask;
    float fcross;

    // store values in longs
    memcpy(&l1,&f1,sizeof(long));
    memcpy(&l2,&f2,sizeof(long));

    do  {
        // create mask
        mask   = 0xFFFFFFFFL << size_t(devgen() * 32.0F);

        // generate offspring
        lcross = (l1 & mask) | (l2 & (~mask));
        }
    while ((lcross & FExpt) == FExpt);

    // copy result to float and return
    memcpy(&fcross,&lcross,sizeof(float));

    return fcross;
    }

double Crossover
    (
    const double & d1,
    const double & d2
    )
    {
    // mask for exponent bits
    static const long DExpt = 0x7FF00000L;

    long   l1[2], l2[2], lcross[2], mask, bit;
    double fcross;

    // store values in longs
    memcpy(l1,&d1,sizeof(double));
    memcpy(l2,&d2,sizeof(double));
	
	cout << "l1[0] = " << l1[0] << ", l1[1] = " << l1[1] << endl;
	cout << "l2[0] = " << l2[0] << ", l2[1] = " << l2[1] << endl;

    do  {
        // calculate bit position for flip
        bit = size_t(devgen() * 64.0F);
	cout << "bit = " << bit << endl;

        if (bit > 31) // if flip in high-order word
            {
            // create mask
            mask   = 0xFFFFFFFFL << int(bit - 32L);
	    cout << "bit > 31: mask = " << mask << endl;

            // duplicate low-order word of first parent
            lcross[0] = l1[0];

            // crossover in high-order word
            lcross[1] = (l1[1] & mask) | (l2[1] & (~mask));
            }
        else
            {
            // create mask
            mask   = 0xFFFFFFFFL << int(bit);
	    cout << "bit <= 31: mask = " << mask << endl;

            // crossover in low-order word
            lcross[0] = (l1[0] & mask) | (l2[0] & (~mask));

            // duplicate high-order word of first parent
            lcross[1] = l1[1];
            }
        }
    while ((lcross[1] & DExpt) == DExpt);

    cout << "lcross[0] = " << lcross[0] << ", lcross[1] = " << lcross[1] << endl;

    // copy and return
    memcpy(&fcross,lcross,sizeof(double));

    return fcross;
    }

