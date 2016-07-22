//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgeev.cpp     v1.00
//
//      Prisoner's Dilemma by evolving finite state machines
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
#include "fsm.h"      // finite state machines
#include "evoptcfg.h" // configuration dialog box

//------------------------------------
// Test evolving finite state machines
//------------------------------------

void TestEvolvingMachines
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    buffer << "Evolving Finite State Machines\r\n"
              "------------------------------\r\n";

    // create configuration and verify it
    EVOptConfig evoc(inst,wdw);

    if (!evoc.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // data arrays
    const size_t    SSZ = 2;
    const size_t    NSZ = 3;
    char  SSET[2] = { 'C', 'D' };
    const size_t POP_SZ = evoc.GetPopSize();

    // variables
    size_t n, i, j, k, inc;
    double vf;
    char buf[64], mj, mk, tj;

    EvolvingFSM<char,SSZ,char,SSZ> * vm;

    RouletteWheel<double> * rw;

    RandDev devgen;

    FSM_TranData<char> td[NSZ][SSZ];

    EVFSM_MuteWts wts =
        {
        evoc.GetWeightO(),
        evoc.GetWeightT(),
        evoc.GetWeightA(),
        evoc.GetWeightD(),
        evoc.GetWeightN()
        };

    // display parameters
    buffer << "\r\n      Population: " << evoc.GetPopSize();
    buffer << "\r\n       Test Size: " << evoc.GetTestSize();
    buffer << "\r\n     Test Length: " << evoc.GetTestLen();
    buffer << "\r\n       Max State: " << evoc.GetMaxState();
    buffer << "\r\n   Mutation Rate: " << evoc.GetMuteRate();
    buffer << "\r\n";
    buffer << "\r\nOutput Symbol Wt: " << evoc.GetWeightO();
    buffer << "\r\n   Transition Wt: " << evoc.GetWeightT();
    buffer << "\r\n    Add State Wt: " << evoc.GetWeightA();
    buffer << "\r\n Delete State Wt: " << evoc.GetWeightD();
    buffer << "\r\nNew InitState Wt: " << evoc.GetWeightN();
    buffer << "\r\n";
    buffer << "\r\n       Payoff DD: " << evoc.GetPayoffDD();
    buffer << "\r\n       Payoff DC: " << evoc.GetPayoffDC();
    buffer << "\r\n       Payoff CD: " << evoc.GetPayoffCD();
    buffer << "\r\n       Payoff CC: " << evoc.GetPayoffCC();
    buffer << "\r\n";
    buffer << "\r\n      First Move: ";

    switch (evoc.GetFirstMove())
        {
        case EVM_DEFECT:
            buffer << "Always Defect";
            break;
        case EVM_COOPER:
            buffer << "Always Cooperate";
            break;
        case EVM_RANDOM:
            buffer << "Random";
        }

    if (evoc.GetFitScale())
        {
        buffer << "\r\n";
        buffer << "\r\n    Fitness Base: " << evoc.GetFitLinBase();
        buffer << "\r\n    Fitness  Dec: " << evoc.GetFitLinDec();
        buffer << "\r\n    Fitness  Min: " << evoc.GetFitLinMin();
        }

    buffer << "\r\n";

    // allocate buffers
    EvolvingFSM<char,SSZ,char,SSZ> ** pop = new EvolvingFSM<char,SSZ,char,SSZ> * [POP_SZ];

    if (pop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    EvolvingFSM<char,SSZ,char,SSZ> ** newpop = new EvolvingFSM<char,SSZ,char,SSZ> * [POP_SZ];

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
    EvolvingFSM<char,SSZ,char,SSZ> * * ptrm = pop - 1;
    double * ptrf = fit   - 1;

    // create an initial population of FSMs
    for (j = 0; j < POP_SZ; ++j)
        {
        for (n = 0; n < NSZ; ++n)
            {
            for (i = 0; i < SSZ; ++i)
                {
                td[n][i].NextState = size_t(devgen() * float(NSZ));

                if (devgen() < 0.5F)
                    td[n][i].Osym = SSET[0];
                else
                    td[n][i].Osym = SSET[1];
                }
            }

        pop[j] = new EvolvingFSM<char,SSZ,char,SSZ> (NSZ,&td[0][0],SSET,SSET,0,wts);

        if (pop[j] == NULL)
            {
            buffer << "Memory allocation failed\r\n";
            return;
            }
        }

    size_t g = 0;

    while (1)
        {
        // display progress in app header
        wsprintf(buf,"Forge (loop: %u of %u)",g,evoc.GetTestSize());
        SetWindowText(wdw,buf);

        // reset FSMs and clear fitness array
        for (j = 0; j < POP_SZ; ++j)
            {
            pop[j]->Reset();
            fit[j] = 0.0;
            }

        // fitness testing by competition
        for (j = 0; j < POP_SZ; ++j)
            {
            for (k = j; k < POP_SZ; ++k)
                {
                // reset machines to initial state
                pop[j]->Reset();
                pop[k]->Reset();

                // select first move
                switch (evoc.GetFirstMove())
                    {
                    case EVM_DEFECT:
                        mj = 'D';
                        mk = 'D';
                        break;
                    case EVM_COOPER:
                        mj = 'C';
                        mk = 'C';
                        break;
                    case EVM_RANDOM:
                        if (devgen() < 0.5F)
                            mj = 'C';
                        else
                            mj = 'D';

                        if (devgen() < 0.5F)
                            mk = 'C';
                        else
                            mk = 'D';
                    }

                n = 1;

                // calculate fitness based on moves
                while (1)
                    {
                    if (mj == SSET[0])
                        {
                        if (mk == SSET[0])
                            {
                            fit[j] += evoc.GetPayoffCC();
                            fit[k] += evoc.GetPayoffCC();
                            }
                        else
                            {
                            fit[j] += evoc.GetPayoffCD();
                            fit[k] += evoc.GetPayoffDC();
                            }
                        }
                    else
                        {
                        if (mk == SSET[0])
                            {
                            fit[j] += evoc.GetPayoffDC();
                            fit[k] += evoc.GetPayoffCD();
                            }
                        else
                            {
                            fit[j] += evoc.GetPayoffDD();
                            fit[k] += evoc.GetPayoffDD();
                            }
                        }

                    // exit if done
                    if (n == evoc.GetTestLen())
                        break;


                    // state transitions based on opponents move
                    tj = pop[j]->Transition(mk);
                    mk = pop[k]->Transition(mj);
                    mj = tj;

                    // next contest
                    ++n;
                    }
                }
            }

        // sort for selecting best
        for (inc = 1; inc <= POP_SZ / 9; inc = 3 * inc + 1) ;

        for ( ; inc > 0; inc /= 3)
            {
            for (i = inc + 1; i <= POP_SZ; i += inc)
                {
                vf = ptrf[i];
                vm = ptrm[i];

                j  = i;

                while ((j > inc) && (ptrf[j - inc] < vf))
                    {
                    ptrf[j] = ptrf[j - inc];
                    ptrm[j] = ptrm[j - inc];

                    j -= inc;
                    }

                ptrf[j] = vf;
                ptrm[j] = vm;
                }
            }

        // display best machine
        if (((g % evoc.GetReptFreq()) == 0)
        ||  (g == evoc.GetTestSize()))
            {
            buffer << "\r\nGeneration " << g
                   << "\r\n----------------\r\n";

            pop[0]->DumpStructure(buffer);

            buffer << "      Fitness: " << fit[0] << "\r\n";
            }

        // exit, if it's time
        if (g == evoc.GetTestSize())
            break;

        // fitness scaling (linear normalization)
        if (evoc.GetFitScale())
            {
            fit[0] = evoc.GetFitLinBase();
            i      = 1;

            while (fit[i-1] <= evoc.GetFitLinDec())
                {
                fit[i] = fit[i-1] - evoc.GetFitLinDec();
                ++i;
                }

            for (; i < POP_SZ; ++i)
                fit[i] = evoc.GetFitLinMin();
            }

        // reproduce new members
        rw = new RouletteWheel<double> (POP_SZ,fit);

        if (rw == NULL)
            {
            buffer << "Memory allocation failed\r\n";
            return;
            }

        for (j = 0; j < POP_SZ; ++j)
            {
            i = rw->GetIndex();

            newpop[j] = new EvolvingFSM<char,SSZ,char,SSZ>(*(pop[i]));

            if (newpop[j] == NULL)
                {
                buffer << "Memory allocation failed\r\n";
                return;
                }

            if (devgen() < evoc.GetMuteRate())
                newpop[j]->Mutate(2,evoc.GetMaxState());
            }

        delete rw;

        // copy next generation and repeat!
        for (j = 0; j < POP_SZ; ++j)
            delete pop[j];

        memcpy(pop,newpop,POP_SZ * sizeof(EvolvingFSM<char,SSZ,char,SSZ> *));

        ++g;
        }

    // delete buffers
    for (j = 0; j < POP_SZ; ++j)
         delete pop[j];

    delete [] pop;
    delete [] newpop;
    delete [] fit;

    // restore window text
    SetWindowText(wdw,"Forge");
    }

