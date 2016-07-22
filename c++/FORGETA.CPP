//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgeta.cpp     v1.00
//
//      Travelling Artist Problem by genetic algorithm,
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "windows.h"  // Windows definitions
#include "forgeres.h" // resource constants
#include "strstrea.h" // strstream definitions
#include "iomanip.h"  // stream manipulators
#include "string.h"   // memory management functions
#include "randdev.h"  // uniform random deviate generator
#include "roulette.h" // RouletteWheel class
#include "bool.h"     // ANSI-like 'bool' class
#include "taoptcfg.h" // configuration dialog box

//-------------------------------
// Test Travelling Artist Problem
//-------------------------------

typedef size_t CityChrom[10];

size_t TAFindCity
    (
    const CityChrom c,
    const size_t city
    )
    {
    // note: no safety catches for out-of-range cities!
    size_t i = 0;

    for (;;)
        {
        if (city == c[i]) return i;
        ++i;
        }
    }

void TestArtist
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    buffer << "Travelling Artist Problem\r\n"
              "-------------------------\r\n";

    // create configuration and verify it
    TAOptConfig taoc(inst,wdw);

    if (!taoc.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // data arrays
    static const size_t CSZ = 10 * sizeof(size_t);

    const size_t POP_SZ = taoc.GetPopSize();

    static const char * cityName[10] =
        {
        "Alamosa  ", "Cortez   ",
        "Creede   ", "Durango  ",
        "Gunnison ", "Lake City",
        "Montrose ", "Pagosa   ",
        "Salida   ", "Silverton"
        };

    static const double distance[10][10] =
        {
        {  0.0,220.0, 90.0,155.0,133.0,123.0,182.0, 89.0,105.0,141.0},
        {220.0,  0.0,135.0, 55.0,173.0,117.0,124.0,122.0,222.0, 85.0},
        { 90.0,135.0,  0.0, 92.0, 69.0, 34.0, 95.0, 56.0, 98.0, 57.0},
        {155.0, 55.0, 92.0,  0.0,145.0, 84.0,116.0, 68.0,184.0, 54.0},
        {133.0,173.0, 69.0,145.0,  0.0, 60.0, 72.0,125.0, 70.0, 91.0},
        {123.0,117.0, 34.0, 84.0, 60.0,  0.0, 61.0, 76.0,106.0, 33.0},
        {182.0,124.0, 95.0,116.0, 72.0, 61.0,  0.0,134.0,137.0, 66.0},
        { 89.0,122.0, 56.0, 68.0,125.0, 76.0,134.0,  0.0,142.0, 73.0},
        {105.0,222.0, 98.0,184.0, 70.0,106.0,137.0,142.0,  0.0,139.0},
        {141.0, 85.0, 57.0, 54.0, 91.0, 33.0, 66.0, 73.0,139.0,  0.0}
        };

    // display parameters
    buffer << "\r\n      Population: " << taoc.GetPopSize();
    buffer << "\r\n       Test Size: " << taoc.GetTestSize();
    buffer << "\r\n     Report Freq: " << taoc.GetReptFreq();
    buffer << "\r\n       Show Best: " << taoc.GetShowBest();
    buffer << "\r\nOper Probability: " << taoc.GetOperChance();

    if (taoc.GetMutation())
        buffer << "\r\n        Mutation: " << taoc.GetWeightM();

    if (taoc.GetInversion())
        buffer << "\r\n       Inversion: " << taoc.GetWeightI();

    if (taoc.GetPMX())
        buffer << "\r\nPart Match Cross: " << taoc.GetWeightP();

    if (taoc.GetCX())
        buffer << "\r\n Cycle Crossover: " << taoc.GetWeightC();

    if (taoc.GetOX())
        buffer << "\r\n Order Crossover: " << taoc.GetWeightO();

    buffer << "\r\n    Fitness Base: " << taoc.GetFitLinBase();
    buffer << "\r\n    Fitness  Dec: " << taoc.GetFitLinDec();
    buffer << "\r\n    Fitness  Min: " << taoc.GetFitLinMin();
    buffer << dec << "\r\n";

    // create working buffers
    CityChrom * pop = new CityChrom [POP_SZ];

    if (pop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    CityChrom * newpop = new CityChrom [POP_SZ];

    if (newpop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    double * fit = new double [POP_SZ];

    if (fit == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    // pointers for shell sort
    CityChrom * ptrp = pop - 1;
    double    * ptrf = fit - 1;

    // define variables
    size_t g, i, j, k, l, n, s, t, p1, p2, inc;
    double vf;
    char buf[64];
    CityChrom vp;
    RouletteWheel<double> * rw;
    RandDev devgen;

    // create roulette wheel for operator selection
    double operwt[5];

    if (taoc.GetMutation())
        operwt[0] = taoc.GetWeightM();
    else
        operwt[0] = 0.0;

    if (taoc.GetInversion())
        operwt[1] = taoc.GetWeightI();
    else
        operwt[1] = 0.0;

    if (taoc.GetPMX())
        operwt[2] = taoc.GetWeightP();
    else
        operwt[2] = 0.0;

    if (taoc.GetCX())
        operwt[3] = taoc.GetWeightC();
    else
        operwt[3] = 0.0;

    if (taoc.GetOX())
        operwt[4] = taoc.GetWeightO();
    else
        operwt[4] = 0.0;

    RouletteWheel<double> ow(5,operwt);

    // create initial population with Josephus permutation
    for (i = 0; i < POP_SZ; ++i)
        {
        int plist[10];

        memset(plist,0,CSZ);

        s = size_t(devgen() *  8.0) + 1;
        j = size_t(devgen() * 10.0);

        k = 0;

        while (1)
            {
            pop[i][k] = j;
            plist[j]  = 1;

            if (k == 9)
                break;

            for (l = 0; l < s; ++l)
                {
                do  {
                    ++j; if (j > 9) j = 0;
                    }
                while (plist[j] == 1);
                }

            ++k;
            }
        }

    // main program loop
    g = 0;

    while (1)
        {
        // display progress in app header
        wsprintf(buf,"Forge (loop: %u of %u)",g,taoc.GetTestSize());
        SetWindowText(wdw,buf);

        // fitness testing
        for (i = 0; i < POP_SZ; ++i)
            {
            fit[i] = 0.0;

            for (j = 1; j < 10; ++j)
                fit[i] += distance[pop[i][j-1]][pop[i][j]];
            }

        // shell sort in preparation for linear normalization
        for (inc = 1; inc <= POP_SZ / 9; inc = 3 * inc + 1) ;

        for ( ; inc > 0; inc /= 3)
            {
            for (i = inc + 1; i <= POP_SZ; i += inc)
                {
                vf = ptrf[i];
                memcpy(vp,ptrp[i],CSZ);

                j  = i;

                while ((j > inc) && (ptrf[j - inc] > vf))
                    {
                    ptrf[j] = ptrf[j - inc];
                    memcpy(ptrp[j],ptrp[j - inc],CSZ);

                    j -= inc;
                    }

                ptrf[j] = vf;
                memcpy(ptrp[j],vp,CSZ);
                }
            }

        // report information
        if (( g == taoc.GetTestSize())
        ||  ((g %  taoc.GetReptFreq()) == 0))
            {
            buffer << "\r\nGeneration " << g << "\r\n";

            buffer << "Best: " << cityName[pop[0][0]];

            for (i = 1; i < 10; ++i)
                buffer << "->" << cityName[pop[0][i]];

            buffer << "\r\n";

            for (i = 0; i < taoc.GetShowBest(); ++i)
                {
                buffer << setw(5) << fit[i] << ": ";

                for (j = 0; j < 10; ++j)
                    buffer << setw(2) << pop[i][j];

                buffer << "\r\n";
                }
            }

        // exit, if necessary
        if (g == taoc.GetTestSize())
            break;

        // fitness scaling (linear normalization)
        fit[0] = taoc.GetFitLinBase();
        i      = 1;

        while (1)
            {
            if (fit[i-1] <= taoc.GetFitLinDec())
                break;

            fit[i] = fit[i-1] - taoc.GetFitLinDec();

            ++i;
            }

        for (; i < POP_SZ; ++i)
            fit[i] = taoc.GetFitLinMin();

        // automatic elitist selection
        memcpy(newpop[0],pop[0],CSZ);

        // generate new population
        rw = new RouletteWheel<double> (POP_SZ,fit);

        if (rw == NULL)
            {
            buffer << "Failed to allocate roulette wheel\r\n";
            return;
            }

        for (i = 1; i < POP_SZ; i += 2)
            {
            // get parents
            p1 = rw->GetIndex();

            do  {
                p2 = rw->GetIndex();
                }
            while (p2 == p1);

            // duplicate parents
            memcpy(newpop[i],  pop[p1],CSZ);
            memcpy(newpop[i+1],pop[p2],CSZ);

            // skip rest of loop if no operator picked
            if (devgen() > taoc.GetOperChance())
                continue;

            // pick an operator
            switch (ow.GetIndex())
                {
                case 0: // mutation
                    for (n = 0; n < 2; ++n)
                        {
                        // pick indexes
                        j = size_t(devgen() * 10.0F);

                        do  {
                            k = size_t(devgen() * 10.0F);
                            }
                        while (k == j);

                        // swap city indexes
                        t = newpop[i+n][k];
                        newpop[i+n][k] = newpop[i+n][j];
                        newpop[i+n][j] = t;
                        }

                    break;

                case 1: // inversion
                    for (n = 0; n < 2; ++n)
                        {
                        // pick indexes
                        j = size_t(devgen() * 9.0F);

                        do  {
                            k = size_t(devgen() * 10.0F);
                           }
                        while (k <= j);

                        // calculate length
                        s = (k - j + 1) / 2;

                        // reverse codes
                        for (l = 0; l < s; ++l)
                            {
                            t = newpop[i+n][k];
                            newpop[i+n][k] = newpop[i+n][j];
                            newpop[i+n][j] = t;

                            ++j;
                            --k;
                            }
                        }

                    break;

                case 2: // partially matched crossover
                    j = size_t(devgen() * 9.0F);

                    do  {
                        k = size_t(devgen() * 10.0F);
                        }
                    while (k <= j);

                    // swap cities
                    for (n = j; n <= k; ++n)
                        {
                        if (pop[p1][n] != pop[p2][n])
                            {
                            s = TAFindCity(newpop[i+1],pop[p1][n]);
                            t = newpop[i+1][n];
                            newpop[i+1][n] = newpop[i+1][s];
                            newpop[i+1][s] = t;

                            s = TAFindCity(newpop[i],pop[p2][n]);
                            t = newpop[i][n];
                            newpop[i][n] = newpop[i][s];
                            newpop[i][s] = t;
                            }
                        }

                    break;

                case 3:
                    // cycle crossover
                    j = size_t(devgen() * 10.0F);
                    t = pop[p1][j];

                    while (1)
                        {
                        newpop[i][j]   = pop[p2][j];
                        newpop[i+1][j] = pop[p1][j];

                        if (newpop[i][j] == t)
                            break;

                        j = TAFindCity(pop[p1],newpop[i][j]);
                        }

                    break;

                case 4:
                    // order crossover
                    j = size_t(devgen() * 9.0F);

                    do  {
                        k = size_t(devgen() * 10.0F);
                        }
                    while (k <= j);

                    if ((j == 0) && (k == 9))
                        {
                        memcpy(vp,newpop[i],CSZ);
                        memcpy(newpop[i],newpop[i+1],CSZ);
                        memcpy(newpop[i+1],vp,CSZ);
                        break;
                        }

                    if (k == 9)
                        n = 0;
                    else
                        n = k + 1;

                    // shift and fill
                    do  {
                        while (1)
                            {
                            s = TAFindCity(pop[p2],newpop[i][n]);

                            if ((s < j) || (s > k))
                                break;

                            // shift members
                            if (n == 9)
                                l = 0;
                            else
                                l = n + 1;

                            while (1)
                                {
                                if (l == 0)
                                    newpop[i][9] = newpop[i][0];
                                else
                                    newpop[i][l-1] = newpop[i][l];

                                if (l == k)
                                    break;

                                if (l == 9)
                                    l = 0;
                                else
                                    ++l;
                                }
                            }

                        while (1)
                            {
                            s = TAFindCity(pop[p1],newpop[i+1][n]);

                            if ((s < j) || (s > k))
                                break;

                            // shift members
                            if (n == 9)
                                l = 0;
                            else
                                l = n + 1;

                            while (1)
                                {
                                if (l == 0)
                                    newpop[i+1][9] = newpop[i+1][0];
                                else
                                    newpop[i+1][l-1] = newpop[i+1][l];

                                if (l == k)
                                    break;

                                if (l == 9)
                                    l = 0;
                                else
                                    ++l;
                                }
                            }

                        if (n == 9)
                            n = 0;
                        else
                            ++n;
                        }
                    while (n != j);

                    for (n = j; n <= k; ++n)
                        {
                        newpop[i][n]   = pop[p2][n];
                        newpop[i+1][n] = pop[p1][n];
                        }

                    break;
                }
            }

        // delete, copy and loop
        delete rw;

        memcpy(pop,newpop,10 * CSZ);

        ++g;
        }

    // delete buffers
    delete [] fit;
    delete [] newpop;
    delete [] pop;

    // restore window text
    SetWindowText(wdw,"Forge");
    }

