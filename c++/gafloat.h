//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      gafloat.h     v1.00
//
//      Defines operations for performing mutation and
//      one-point crossover on floating-point values.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_GAFLOAT_H
#define FORGE_GAFLOAT_H

// mutagen
class FloatMutagen
    {
    public:
        FloatMutagen
            (
            const float & sweight =  5.0,
            const float & eweight =  5.0,
            const float & mweight = 90.0
            );

	float MPick
	    (
	    );
        float Mutate
            (
            const float & f
            );

        double Mutate
            (
            const double & d
            );

    protected:
        const float TotalW;
        const float SignW;
        const float ExpW;
    };

// crossover prototypes
float Crossover
    (
    const float & f1,
    const float & f2
    );

double Crossover
    (
    const double & d1,
    const double & d2
    );

#endif

