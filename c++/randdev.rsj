//=============================================================
//  Random Number Classes   randdev.h    v1.10
//      "Random number" generator that produces statistically-
//      excellent uniform deviates, modified from code
//      published in Chap. 7 of NUMERICAL RECIPES IN C, 2nd ed.
//      Copyright 1992, 1995 by Scott Robert Ladd.
//=============================================================

#ifndef RANDDEV_H
#define RANDDEV_H

#include "stddef.h"
#include "time.h"

class RandDev
    {
    protected:
        // used to set default seed argument from system time
        static long TimeSeed()
            {
            return (long)time(NULL);
            }

    public:
        // constructor
        RandDev
            (
            long initSeed = TimeSeed()
            );

        // set seed value
        void SetSeed
            (
            long newSeed = TimeSeed()
            );

        // get a psuedo-random number between 0.0 and 1.0
        float operator () ();

    private:
        long Seed;
    };

inline RandDev::RandDev
    (
    long initSeed
    )
    {
    if (initSeed < 0)
        Seed = initSeed;
    else
        Seed = -initSeed;
    }

inline void RandDev::SetSeed
    (
    long initSeed
    )
    {
    if (initSeed < 0)
        Seed = initSeed;
    else
        Seed = -initSeed;
    }

#endif