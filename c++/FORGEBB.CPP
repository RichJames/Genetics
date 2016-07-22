//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgebb.cpp     v1.00
//
//      Blackbox optimization
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "windows.h"  // Windows definitions
#include "forgeres.h" // resource constants
#include "strstrea.h" // strstream definitions
#include "iomanip.h"  // stream manipulators
#include "math.h"     // math functions
#include "bool.h"     // ANSI-like 'bool' class
#include "limits.h"   // limits of types
#include "stdio.h"    // standard file I/O functions
#include "stdlib.h"   // misc. library functions
#include "time.h"     // time functions and types

//------------------
// class definitions
//------------------

class BBOptConfig
    {
    public:
        BBOptConfig
            (
            HINSTANCE inst,
            HWND      parent
            );

        static void DlgInit
            (
            HWND  dlg
            );

        static BOOL DlgStore
            (
            HWND  dlg
            );

        // interrogation
        bool   GetValidity()  { return Valid;     }
        size_t GetPopSize()   { return PopSize;   }
        size_t GetTestSize()  { return TestSize;  }
        bool   GetCrossover() { return Crossover; }
        float  GetCrossProb() { return CrossProb; }
        bool   GetMutate()    { return Mutate;    }
        float  GetMuteProb()  { return MuteProb;  }
        bool   GetScaling()   { return Scaling;   }
        bool   GetElitist()   { return Elitist;   }

    private:
        // parameters
        bool   Valid;
        size_t PopSize;
        size_t TestSize;
        bool   Crossover;
        float  CrossProb;
        bool   Mutate;
        float  MuteProb;
        bool   Scaling;
        bool   Elitist;

        // stored parameter values
        static size_t DefPopSize;
        static size_t DefTestSize;
        static bool   DefCrossover;
        static float  DefCrossProb;
        static bool   DefMutate;
        static float  DefMuteProb;
        static bool   DefScaling;
        static bool   DefElitist;
    };

size_t BBOptConfig::DefPopSize   =   25;
size_t BBOptConfig::DefTestSize  =  100;
bool   BBOptConfig::DefCrossover = true;
float  BBOptConfig::DefCrossProb =    1.00F;
bool   BBOptConfig::DefMutate    = true;
float  BBOptConfig::DefMuteProb  =    0.85F;
bool   BBOptConfig::DefScaling   = true;
bool   BBOptConfig::DefElitist   = true;

static LPCSTR BBOptConfigDlgName = MAKEINTRESOURCE(DLG_BLACKBOX);

static const char * BBOptConfigDlgErr =
    "Your input is in error!\n\n"
    "Population  must be >= 10.\n"
    "Generations must be > 0.";

BOOL CALLBACK BBOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

BBOptConfig::BBOptConfig
    (
    HINSTANCE inst,
    HWND      parent
    )
    {
    DLGPROC thunk = MakeProcInstance(FARPROC(BBOptConfigDlgProc),inst);
    int res = DialogBox(inst,BBOptConfigDlgName,parent,thunk);
    FreeProcInstance(thunk);

    if (res)
        Valid = true;
    else
        Valid = false;

    PopSize   = DefPopSize;
    TestSize  = DefTestSize;
    Crossover = DefCrossover;
    CrossProb = DefCrossProb;
    Mutate    = DefMutate;
    MuteProb  = DefMuteProb;
    Scaling   = DefScaling;
    Elitist   = DefElitist;
    }

#ifdef __BORLANDC__
#pragma argsused
#endif

BOOL CALLBACK BBOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    )
    {
    switch (message)
        {
        case WM_INITDIALOG:
            BBOptConfig::DlgInit(dlg);
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
                {
                case IDOK:
                    if (TRUE == BBOptConfig::DlgStore(dlg))
                        EndDialog(dlg, 1);

                    break;

                case IDCANCEL:
                    EndDialog(dlg, 0);
                }

            return TRUE;
        }

    return FALSE;
    }

void BBOptConfig::DlgInit
    (
    HWND  dlg
    )
    {
    char temp[64];

    sprintf(temp,"%u",DefPopSize);
    SetDlgItemText(dlg,IDD_BBOX_POP,temp);

    sprintf(temp,"%u",DefTestSize);
    SetDlgItemText(dlg,IDD_BBOX_GEN,temp);

    sprintf(temp,"%.6g",DefCrossProb * 100.0F);
    SetDlgItemText(dlg,IDD_BBOX_CPROB,temp);

    sprintf(temp,"%.6g",DefMuteProb * 100.0F);
    SetDlgItemText(dlg,IDD_BB0X_MPROB,temp);

    if (DefCrossover)
        SendDlgItemMessage(dlg,IDD_BBOX_CROSS,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_BBOX_CROSS,BM_SETCHECK,0,0L);

    if (DefMutate)
        SendDlgItemMessage(dlg,IDD_BBOX_MUTE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_BBOX_MUTE,BM_SETCHECK,0,0L);

    if (DefScaling)
        SendDlgItemMessage(dlg,IDD_BBOX_FSCALE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_BBOX_FSCALE,BM_SETCHECK,0,0L);

    if (DefElitist)
        SendDlgItemMessage(dlg,IDD_BBOX_ELITISM,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_BBOX_ELITISM,BM_SETCHECK,0,0L);
    }

BOOL BBOptConfig::DlgStore
    (
    HWND  dlg
    )
    {
    char temp[64];

    GetDlgItemText(dlg,IDD_BBOX_POP,temp,64);
    size_t psz = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_BBOX_GEN,temp,64);
    size_t gen = abs(atoi(temp));

    if ((psz < 10) || (gen == 0))
        {
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL,BBOptConfigDlgErr,"Configuration Error",MB_OK | MB_ICONHAND);
        return FALSE;
        }

    DefPopSize  = psz;
    DefTestSize = gen;

    GetDlgItemText(dlg,IDD_BBOX_CPROB,temp,64);
    DefCrossProb = float(fabs(atof(temp))) / 100.0F;

    GetDlgItemText(dlg,IDD_BB0X_MPROB,temp,64);
    DefMuteProb = float(fabs(atof(temp))) / 100.0F;

    if (SendDlgItemMessage(dlg,IDD_BBOX_CROSS,BM_GETCHECK,0,0L))
        DefCrossover = true;
    else
        DefCrossover = false;

    if (SendDlgItemMessage(dlg,IDD_BBOX_MUTE,BM_GETCHECK,0,0L))
        DefMutate = true;
    else
        DefMutate = false;

    if (SendDlgItemMessage(dlg,IDD_BBOX_FSCALE,BM_GETCHECK,0,0L))
        DefScaling = true;
    else
        DefScaling = false;

    if (SendDlgItemMessage(dlg,IDD_BBOX_ELITISM,BM_GETCHECK,0,0L))
        DefElitist = true;
    else
        DefElitist = false;

    return TRUE;
    }

//----------------------
// Blackbox optimization
//----------------------

long Blackbox
    (
    long x
    )
    {
    // test value -- the speed of light in meters per second
    static const long n =  0x11DE784AL; // 299,792,458;

    long fit  = 0L;
    long mask = 1L;

    // count matching bits
    for (int i = 0; i < 32; ++i)
        {
        if ((x & mask) == (n & mask))
            ++fit;

        mask <<= 1;
        }

    // return fitness between 0 and 32
    return fit;
    }

void TestBlackbox
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    // display header
    buffer << "Blackbox Optimization\r\n"
              "---------------------\r\n\r\n"
           << setprecision(7);

    // get configuration and verify it
    BBOptConfig cfg(inst,wdw);

    if (!cfg.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // display parameters for this run
    buffer << "Pop. Size: " << cfg.GetPopSize()   << "\r\n";
    buffer << "Test Size: " << cfg.GetTestSize()  << "\r\n";
    buffer << "Crossover: " << cfg.GetCrossover() << " (" << cfg.GetCrossProb() * 100.0F << "%)\r\n";
    buffer << " Mutation: " << cfg.GetMutate()    << " (" << cfg.GetMuteProb()  * 100.0F << "%)\r\n";
    buffer << "  Scaling: " << cfg.GetScaling()   << "\r\n";
    buffer << "  Elitism: " << cfg.GetElitist()   << "\r\n\r\n";

    // get dimensions
    const size_t POP_SZ = cfg.GetPopSize();
    const size_t GEN_SZ = cfg.GetTestSize();
    const bool   cross  = cfg.GetCrossover();
    const float  crate  = cfg.GetCrossProb();
    const bool   mutate = cfg.GetMutate();
    const float  mrate  = cfg.GetMuteProb();
    const bool   elite  = cfg.GetElitist();
    const bool   scale  = cfg.GetScaling();

    // initialize psuedo-random number generator
    srand((unsigned)time(NULL));

    // allocate population and fitness buffers
    long * pop = new long[POP_SZ];

    if (pop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    long * newpop = new long[POP_SZ];

    if (newpop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    long * fit = new long[POP_SZ];

    if (fit == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    // various variables
    long bestl, bestf, minf, mask, sel, totf, avgf;
    size_t i, g, p1, p2;
    char buf[64];

    // create initial population
    for (i = 0; i < POP_SZ; ++i)
        pop[i] = long(rand());

    // start with generation zero
    g = 0;

    while (1) // loop breaks in middle
        {
        // display progress
        wsprintf(buf,"Forge (loop: %5u of %5u)",g,GEN_SZ);
        SetWindowText(wdw,buf);

        // initialize for fitness testing
        bestf = -1L;
        totf  = 0L;
        minf  = LONG_MAX;

        // fitness testing
        for (i = 0; i < POP_SZ; ++i)
            {
            // call fitness function and store result
            fit[i] = Blackbox(pop[i]);

            // keep track of best fitness
            if (fit[i] > bestf)
                {
                bestf = fit[i];
                bestl = pop[i];
                }

            // keep track of least fit
            if (fit[i] < minf)
                minf = fit[i];

            // total fitness
            totf += fit[i];
            }

        // make sure we have at least some fit values
        if (totf == 0L)
            {
            buffer << "Population has total fitness of ZERO\r\n";
            return;
            }

        // compute average fitness
        avgf = totf / POP_SZ;

        // sum (and maybe scale) fitness values
        if (scale)
            {
            // ensures that the least fitness is one
            ++minf;

            // recalculate total fitness to reflect scaled values
            totf = 0L;

            for (i = 0; i < POP_SZ; ++i)
                {
                fit[i] -= minf;   // reduce by smallest fitness
                fit[i] *= fit[i]; // square result of above
                totf   += fit[i]; // add into total fitness
                }
            }

        // display stats for this generation
        buffer << setw(4) << g
               << " best: " << setw(8) << hex << bestl
               << " (" << setw(2) << dec << bestf
               << ") avg. fit = " << setw(2) << avgf
               << " min. fit = "  << setw(2) << minf << "\r\n";

        // exit if this is final generation
        if (g == GEN_SZ)
            break;

        // create new population
        for (i = 0; i < POP_SZ; ++i)
            {
            // roulette-select parent
            sel = (long)((float(rand()) / float(RAND_MAX)) * float(totf));
            p1  = 0;

            while (sel > fit[p1])
                {
                sel -= fit[p1];
                ++p1;
                }

            // crossover reproduction
            if (cross && ((float(rand()) / float(RAND_MAX)) < crate))
                {
                // roulette-select second parent
                sel = (long)((float(rand()) / float(RAND_MAX)) * float(totf));
                p2 = 0;

                while (sel > fit[p2])
                    {
                    sel -= fit[p2];
                    ++p2;
                    }

                // mask of bits to be copied from first parent
                mask = 0xFFFFFFFFL << (int)((float(rand()) / float(RAND_MAX)) * 32.0F);

                // new string from two parents
                newpop[i] = (pop[p1] & mask) | (pop[p2] & (~mask));
                }
            else
                // one parent, no crossover reproduction
                newpop[i] = pop[p1];

            // mutation
            if (mutate && ((float(rand()) / float(RAND_MAX)) < mrate))
                {
                // select bit to be changed
                mask = 1L << (int)((float(rand()) / float(RAND_MAX)) * 32.0F);

                // flip the bit
                if (newpop[i] & mask)
                    newpop[i] &= ~mask;
                else
                    newpop[i] |= mask;
                }
            }

        // if elitist selection, replace first item with best
        if (elite)
            newpop[0] = bestl;

        // replace old population with new one
        memcpy(pop,newpop,POP_SZ * sizeof(long));

        // increment generation
        ++g;
        }

    // set window header to application name
    SetWindowText(wdw,"Forge");

    // delete population and fitness arrays
    delete [] pop;
    delete [] newpop;
    delete [] fit;
    }

//---------------------------------
// Blackbox Optimization (Analysis)
//---------------------------------

void TestBBAnalysis
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    buffer << "Blackbox Optimization (Analysis)\r\n"
              "--------------------------------\r\n\r\n"
           << setprecision(7);

    // get configuration and verify it
    BBOptConfig cfg(inst,wdw);

    if (!cfg.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // display parameters for this run
    buffer << "Pop. Size: " << cfg.GetPopSize()   << "\r\n";
    buffer << "Test Size: " << cfg.GetTestSize()  << "\r\n";
    buffer << "Crossover: " << cfg.GetCrossover() << " (" << cfg.GetCrossProb() * 100.0F << "%)\r\n";
    buffer << " Mutation: " << cfg.GetMutate()    << " (" << cfg.GetMuteProb()  * 100.0F << "%)\r\n";
    buffer << "  Scaling: " << cfg.GetScaling()   << "\r\n";
    buffer << "  Elitism: " << cfg.GetElitist()   << "\r\n\r\n";

    // get dimensions
    const size_t POP_SZ = cfg.GetPopSize();
    const size_t GEN_SZ = cfg.GetTestSize();
    const size_t ANL_SZ = 100U;
    const bool   cross  = cfg.GetCrossover();
    const float  crate  = cfg.GetCrossProb();
    const bool   mutate = cfg.GetMutate();
    const float  mrate  = cfg.GetMuteProb();
    const bool   elite  = cfg.GetElitist();
    const bool   scale  = cfg.GetScaling();

    // initialize psuedo-random number generator
    srand((unsigned)time(NULL));

    // allocate population and fitness buffers
    long * pop = new long[POP_SZ];

    if (pop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    long * newpop = new long[POP_SZ];

    if (newpop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    long * fit = new long[POP_SZ];

    if (fit == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    // various variables
    long bestl, bestf, minf, mask, sel, totf;
    long sumg = 0L, sumt = 0L;
    size_t a, i, g, p1, p2, ming = UINT_MAX, maxg = 0;
    clock_t start, elapsed, mint = UINT_MAX, maxt = 0;
    char buf[64];

    for (a = 0; a < ANL_SZ; ++a)
        {
        // get starting time
        start = clock();

        // create initial population
        for (i = 0; i < POP_SZ; ++i)
            pop[i] = long(rand());

        // start with generation zero
        g = 0;

        while (1) // loop breaks in middle
            {
            // display progress
            wsprintf(buf,"Forge (analysis %5u of %u, loop %5u of %u)",
                     a,ANL_SZ,g,GEN_SZ);
            SetWindowText(wdw,buf);

            // initialize for fitness testing
            bestf = -1L;
            totf  =  0L;
            minf  = LONG_MAX;

            // fitness testing
            for (i = 0; i < POP_SZ; ++i)
                {
                // call fitness function and store result
                fit[i] = Blackbox(pop[i]);

                // keep track of best fitness
                if (fit[i] > bestf)
                    {
                    bestf = fit[i];
                    bestl = pop[i];
                    }

                // keep track of least fit
                if (fit[i] < minf)
                    minf = fit[i];

                // total fitness
                totf += fit[i];
                }

            // exit if this is final generation
            if ((totf == 0L) || (bestf == 32L) || (g == GEN_SZ))
                break;

            // sum (and maybe scale) fitness values
            if (scale)
                {
                // ensures that the least fitness is one
                ++minf;

                // recalculate total fitness to reflect scaled values
                totf = 0L;

                for (i = 0; i < POP_SZ; ++i)
                    {
                    fit[i] -= minf;   // reduce by smallest fitness
                    fit[i] *= fit[i]; // square result of above
                    totf   += fit[i]; // add into total fitness
                    }
                }

            // create new population
            for (i = 0; i < POP_SZ; ++i)
                {
                // roulette-select parent
                sel = (long)((float(rand()) / float(RAND_MAX)) * float(totf));
                p1  = 0;

                while (sel > fit[p1])
                    {
                    sel -= fit[p1];
                    ++p1;
                    }

                // crossover reproduction
                if (cross && ((float(rand()) / float(RAND_MAX)) < crate))
                    {
                    // roulette-select second parent
                    sel = (long)((float(rand()) / float(RAND_MAX)) * float(totf));
                    p2 = 0;

                    while (sel > fit[p2])
                        {
                        sel -= fit[p2];
                        ++p2;
                        }

                    // mask of bits to be copied from first parent
                    mask = 0xFFFFFFFFL << (int)((float(rand()) / float(RAND_MAX)) * 32.0F);

                    // new string from two parents
                    newpop[i] = (pop[p1] & mask) | (pop[p2] & (~mask));
                    }
                else
                    // one parent, no crossover reproduction
                    newpop[i] = pop[p1];

                // mutation
                if (mutate && ((float(rand()) / float(RAND_MAX)) < mrate))
                    {
                    // select bit to be changed
                    mask = 1L << (int)((float(rand()) / float(RAND_MAX)) * 32.0F);

                    // flip the bit
                    if (newpop[i] & mask)
                        newpop[i] &= ~mask;
                    else
                        newpop[i] |= mask;
                    }
                }

            // if elitist selection, replace first item with best
            if (elite)
                newpop[0] = bestl;

            // replace old population with new one
            memcpy(pop,newpop,POP_SZ * sizeof(long));

            // increment generation
            ++g;
            }

        // calculate elapsed time
        elapsed = clock() - start;
        sumt += elapsed;

        if (elapsed < mint) mint = elapsed;
        if (elapsed > maxt) maxt = elapsed;

        if (g < ming) ming = g;
        if (g > maxg) maxg = g;

        sumg += g;
        }

    // report statistics
    buffer << "gen  average = "
           << (float(sumg) / float(ANL_SZ)) << "\r\n";
    buffer << "gen  minimum = " << ming << "\r\n";
    buffer << "gen  maximum = " << maxg << "\r\n\r\n";

    buffer << "tick average = "
           << (float(sumt) / float(ANL_SZ)) << "\r\n";
    buffer << "tick minimum = " << mint << "\r\n";
    buffer << "tick maximum = " << maxt << "\r\n";

    // set window header to application name
    SetWindowText(wdw,"Forge");

    // delete population and fitness arrays
    delete [] pop;
    delete [] newpop;
    delete [] fit;
    }

