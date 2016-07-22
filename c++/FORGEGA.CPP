//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgega.cpp     v1.00
//
//      Function optimization via genetic algorithm
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "windows.h"  // Windows definitions
#include "forgeres.h" // resource constants
#include "strstrea.h" // strstream definitions
#include "iomanip.h"  // stream manipulators
#include "randdev.h"  // uniform random deviate generator
#include "roulette.h" // RouletteWheel class
#include "bool.h"     // ANSI-like 'bool' class
#include "gaoptcfg.h" // optimization configuration dialog box
#include "math.h"     // math functions
#include "float.h"    // floating point parameters
#include "fpconst.h"  // floating point constants
#include "fputil.h"   // floating-point utility functions
#include "gafloat.h"  // mutation and crossover for IEEE floats

//------------------------------------
// Function Optimization (Peak Search)
//------------------------------------

template <class T>
    inline T sqr(const T & n) { return n * n; }

inline double FitnessF6
    (
    double x,
    double y
    )
    {
    return  0.7 + sqr(x)
                + 2.0 * sqr(y)
                - 0.3 * cos(3.0 * N_DPI * x)
                - 0.4 * cos(4.0 * N_DPI * y);
    }

inline double FitnessF7
    (
    double x,
    double y
    )
    {
    return  0.3 + sqr(x)
                + 2.0 * sqr(y)
                - 0.3 * (cos(3.0 * N_DPI * x)
                          * cos(4.0 * N_DPI * y));
    }

inline double FitnessF8
    (
    double x,
    double y
    )
    {
    return  0.3 + sqr(x)
                + 2.0 * sqr(y)
                - 0.3 * (cos(3.0 * N_DPI * x)
                          + cos(4.0 * N_DPI * y));
    }

inline double FitnessCust
    (
    double x,
    double y
    )
    {
    return 1.0 / (0.8 + sqr(x + 0.5)
                      + 2.0 * sqr(y - 0.5)
                      - 0.3 * cos(3.0 * N_DPI * x)
                      - 0.4 * cos(4.0 * N_DPI * y));
    }

void TestPeakSearch
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    buffer << "Function Optimization (Peak Search)\r\n"
              "-----------------------------------\r\n";

    // create configuration and verify it
    GAOptConfig gaoc(inst,wdw);

    if (!gaoc.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // display parameters for this run
    buffer << "\r\n   Equation: ";

    switch (gaoc.GetEquation())
        {
        case 0:
            buffer << "f6(x,y) = x²+2y²-0.3cos(3px)-0.4cos(4py)+0.7";
            break;
        case 1:
            buffer << "f7(x,y) = x²+2y²-0.3[cos(3px)cos(4py)]+0.3";
            break;
        case 2:
            buffer << "f8(x,y) = x²+2y²-0.3[cos(3px)+cos(4py)]+0.3";
            break;
        case 3:
            buffer << "f(x,y) = 1/((x+0.5)²+2(y-0.5)²-0.3cos(3px)-0.4cos(4py)+0.8)";
        }

    buffer << "\r\n  Pop. Size: " << gaoc.GetPopSize();
    buffer << "\r\n  Test Size: " << gaoc.GetTestSize();
    buffer << "\r\n  Rep. Freq: " << gaoc.GetReptFreq();
    buffer << "\r\nSig. Digits: " << gaoc.GetSigDigits();
    buffer << "\r\n      X Min: " << gaoc.GetXMin();
    buffer << "\r\n      X Max: " << gaoc.GetXMax();
    buffer << "\r\n      Y Min: " << gaoc.GetYMin();
    buffer << "\r\n      Y Max: " << gaoc.GetYMax();
    buffer << "\r\nCrossover %: " << gaoc.GetCrossRate() * 100.0F;
    buffer << "\r\n    Cross X: " << gaoc.GetCrossX();
    buffer << "\r\n    Cross Y: " << gaoc.GetCrossY();
    buffer << "\r\n    Cross B: " << gaoc.GetCrossB();
    buffer << "\r\n Mutation %: " << gaoc.GetMuteRate() * 100.0F;
    buffer << "\r\n   Wt. Sign: " << gaoc.GetWtSign();
    buffer << "\r\n   Wt. Expt: " << gaoc.GetWtExp();
    buffer << "\r\n   Wt. Mant: " << gaoc.GetWtMant();
    buffer << "\r\n   Mutate X: " << gaoc.GetMutateX();
    buffer << "\r\n   Mutate Y: " << gaoc.GetMutateY();
    buffer << "\r\nMutate Loop: " << gaoc.GetMuteLoop();
    buffer << "\r\n    Elitism: " << gaoc.GetElitist();
    buffer << "\r\nFit Scaling: " << gaoc.GetFitScale();
    buffer << "\r\n  Fit Algor: ";

    switch(gaoc.GetFitAlgor())
        {
        case FSA_EXPON:
            buffer << "Exponential";
            break;
        case FSA_WINDOW:
            buffer << "Windowing";
            break;
        case FSA_LINEAR:
            buffer << "Linear Normalization";
            buffer << "\r\nFS Lin Base: " << gaoc.GetFitLinBase();
            buffer << "\r\nFS Lin  Dec: " << gaoc.GetFitLinDec();
            buffer << "\r\nFS Lin  Min: " << gaoc.GetFitLinMin();
        }

    buffer << "\r\n\r\n";

    // store dimensions of test
    const size_t POP_SZ  = gaoc.GetPopSize();
    const size_t GEN_SZ  = gaoc.GetTestSize();
    const size_t EQ_ID   = gaoc.GetEquation();
    const size_t SIG_DIG = gaoc.GetSigDigits();

    buffer << setprecision(SIG_DIG) << dec;

    // create random deviate and mutation objects
    RandDev devgen;
    FloatMutagen fmute(gaoc.GetWtSign(),gaoc.GetWtExp(),gaoc.GetWtMant());

    // allocate population and fitness arrays
    double * x = new double [POP_SZ];

    if (x == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    double * xnew = new double [POP_SZ];

    if (xnew == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    double * y = new double [POP_SZ];

    if (y == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    double * ynew = new double [POP_SZ];

    if (ynew == NULL)
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

    double * ptrf = fit - 1;
    double * ptrx =  x  - 1;
    double * ptry =  y  - 1;

    // various variables
    double best, lowf, fitn, vf, vx, vy;
    size_t i, j, inc, g, ibest, p1, p2;
    char buf[64];

    // calculate ranges
    const double rangex = gaoc.GetXMax() - gaoc.GetXMin();
    const double rangey = gaoc.GetYMax() - gaoc.GetYMin();

    // generate initial X values
    for (i = 0; i < POP_SZ; ++i)
        {
        x[i] = SigDig(rangex * devgen() + gaoc.GetXMin(),SIG_DIG);
        y[i] = SigDig(rangey * devgen() + gaoc.GetYMin(),SIG_DIG);
        }

    // do the generations
    for (g = 0; g < GEN_SZ; ++g)
        {
        // display progress in app header
        wsprintf(buf,"Forge (loop: %u of %u)",g,GEN_SZ);
        SetWindowText(wdw,buf);

        // calculate fitness for x values
        best  = DBL_MIN;
        lowf  = DBL_MAX;
        ibest = 0;

        for (i = 0; i < POP_SZ; ++i)
            {
            switch (EQ_ID)
                {
                case 0:
                    fit[i] = 1.0 - FitnessF6(x[i],y[i]);
                    break;
                case 1:
                    fit[i] = 1.0 - FitnessF7(x[i],y[i]);
                    break;
                case 2:
                    fit[i] = 1.0 - FitnessF8(x[i],y[i]);
                    break;
                case 3:
                    fit[i] = FitnessCust(x[i],y[i]);
                }

            fit[i] = SigDig(fit[i],SIG_DIG);

            // track best fitness
            if (fit[i] > best)
                {
                best  = fit[i];
                ibest = i;
                }

            // track lowest fitness
            if (fit[i] < lowf)
                lowf = fit[i];
            }

        // display best solution so far
        if ((g % gaoc.GetReptFreq()) == 0)
            {
            buffer << setw(4) << g << ": (" << x[ibest]
                   << "," << y[ibest] << ") fit = "
                   << best << "\r\n";
            }

        // sort by fitness if linear normalization
        if (FSA_LINEAR == gaoc.GetFitAlgor())
            {
            // shell sort three arrays in order of fitness
            fitn = gaoc.GetFitLinBase();

            for (inc = 1; inc <= POP_SZ / 9; inc = 3 * inc + 1) ;

            for ( ; inc > 0; inc /= 3)
                {
                for (i = inc + 1; i <= POP_SZ; i += inc)
                    {
                    vf = ptrf[i];
                    vx = ptrx[i];
                    vy = ptry[i];

                    j  = i;

                    while ((j > inc) && (ptrf[j - inc] < vf))
                        {
                        ptrf[j] = ptrf[j - inc];
                        ptrx[j] = ptrx[j - inc];
                        ptry[j] = ptry[j - inc];

                        j -= inc;
                        }

                    ptrf[j] = vf;
                    ptrx[j] = vx;
                    ptry[j] = vy;
                    }
                }
            }

        for (i = 0; i < POP_SZ; ++i)
            {
            // fitness scaling
            if (gaoc.GetFitScale())
                {
                switch (gaoc.GetFitAlgor())
                    {
                    case FSA_EXPON:
                        fit[i] = sqr(fit[i] + 1.0);
                        break;
                    case FSA_WINDOW:
                        fit[i] -= lowf;
                        break;
                    case FSA_LINEAR:
                        {
                        fit[i] = fitn;

                        if (fitn > gaoc.GetFitLinMin())
                            {
                            fitn -= gaoc.GetFitLinDec();

                            if (fitn < gaoc.GetFitLinMin())
                                fitn = gaoc.GetFitLinMin();
                            }
                        }
                    }
                }
            }

        // create roulette wheel for reproduction selection
        RouletteWheel<double> * sel;
        sel = new RouletteWheel<double> (POP_SZ,fit);

        if (sel == NULL)
            {
            buffer << "Failed to allocate roulette wheel\r\n";
            return;
            }

        // if elitist, include best from orig. population
        if (gaoc.GetElitist())
            {
            if (FSA_LINEAR == gaoc.GetFitAlgor())
                {
                xnew[0] = x[0];
                ynew[0] = y[0];
                }
            else
                {
                xnew[0] = x[ibest];
                ynew[0] = y[ibest];
                }

            i = 1;
            }
        else
            i = 0;

        // create new population of x's
        for ( ; i < POP_SZ; ++i)
            {
            // create a new x
            p1 = sel->GetIndex();

            if (gaoc.GetCrossX()
            &&  (devgen() <= gaoc.GetCrossRate()))
                {
                p2 = sel->GetIndex();
                xnew[i] = Crossover(x[p1],x[p2]);
                }
            else
                xnew[i] = x[p1];

            // create a new y
            if (gaoc.GetCrossB())
                p1 = sel->GetIndex();

            if (gaoc.GetCrossY()
            &&  (devgen() <= gaoc.GetCrossRate()))
                {
                p2 = sel->GetIndex();
                ynew[i] = Crossover(y[p1],y[p2]);
                }
            else
                ynew[i] = y[p1];

            // mutate X
            if (gaoc.GetMutateX())
                {
                if (gaoc.GetMuteLoop())
                    {
                    while (devgen() <= gaoc.GetMuteRate())
                        xnew[i] = fmute.Mutate(xnew[i]);
                    }
                else
                    {
                    if (devgen() <= gaoc.GetMuteRate())
                        xnew[i] = fmute.Mutate(xnew[i]);
                    }
                }

            // mutate Y
            if (gaoc.GetMutateY())
                {
                if (gaoc.GetMuteLoop())
                    {
                    while (devgen() <= gaoc.GetMuteRate())
                        ynew[i] = fmute.Mutate(ynew[i]);
                    }
                else
                    {
                    if (devgen() <= gaoc.GetMuteRate())
                        ynew[i] = fmute.Mutate(ynew[i]);
                    }
                }

            // make sure x & y fit ranges
            if (xnew[i] > gaoc.GetXMax())
                xnew[i] = gaoc.GetXMax();

            if (xnew[i] < gaoc.GetXMin())
                xnew[i] = gaoc.GetXMin();

            if (ynew[i] > gaoc.GetYMax())
                ynew[i] = gaoc.GetYMax();

            if (ynew[i] < gaoc.GetYMin())
                ynew[i] = gaoc.GetYMin();

            // truncate digits
            xnew[i] = SigDig(xnew[i],SIG_DIG);
            ynew[i] = SigDig(ynew[i],SIG_DIG);
            }

        // remove roulette wheel
        delete sel;

        // copy new population
        memcpy(x,xnew,POP_SZ * sizeof(double));
        memcpy(y,ynew,POP_SZ * sizeof(double));
        }

    // delete buffers
    delete [] fit;
    delete [] ynew;
    delete [] y;
    delete [] xnew;
    delete [] x;

    // restore app window name
    SetWindowText(wdw,"Forge");
    }

