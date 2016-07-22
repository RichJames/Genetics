//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgera.cpp     v1.00
//
//      Evolving robotic ants
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
#include "float.h"    // floating-point dimensions
#include "fsm.h"      // finite state machines
#include "raoptcfg.h" // configuration dialog box

//---------------------------
// Test evolving robotic ants
//---------------------------

void TestRoboticAnts
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    buffer << "Evolving Robotic Ants\r\n"
              "---------------------\r\n";

    // create configuration and verify it
    RAOptConfig raoc(inst,wdw);

    if (!raoc.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // display parameters
    buffer << "\r\n      Population: " << raoc.GetPopSize();
    buffer << "\r\n       Test Size: " << raoc.GetTestSize();
    buffer << "\r\n       Min State: " << raoc.GetMinState();
    buffer << "\r\n       Max State: " << raoc.GetMaxState();
    buffer << "\r\n       Max Moves: " << raoc.GetMaxMoves();
    buffer << "\r\n   Mutation Rate: " << raoc.GetMuteRate();
    buffer << "\r\n";
    buffer << "\r\nOutput Symbol Wt: " << raoc.GetWeightO();
    buffer << "\r\n   Transition Wt: " << raoc.GetWeightT();
    buffer << "\r\n    Add State Wt: " << raoc.GetWeightA();
    buffer << "\r\n Delete State Wt: " << raoc.GetWeightD();
    buffer << "\r\nNew InitState Wt: " << raoc.GetWeightN();

    if (raoc.GetFitScale())
        {
        buffer << "\r\n";
        buffer << "\r\n    Fitness Base: " << raoc.GetFitLinBase();
        buffer << "\r\n    Fitness  Dec: " << raoc.GetFitLinDec();
        buffer << "\r\n    Fitness  Min: " << raoc.GetFitLinMin();
        }

    buffer << "\r\n";

    // data arrays
    const size_t MoveSz = 3;
    const size_t ViewSz = 3;
    const size_t StatSz = raoc.GetMinState();
    const size_t POP_SZ = raoc.GetPopSize();

    char MoveSet[3] =
        {
        'A',    // ahead
        'L',    // left
        'R'     // right
        };

    char ViewSet[3] =
        {
        'F',    // sees food
        'E',    // sees empty square
        'C'     // sees a cliff
        };

    static const size_t GridLen = 40;
    static const size_t GridWid = 11;

    static const int MasterGrid[GridLen][GridWid] =
        {
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0}
        };

    static const int Offset[4][2] =
        {
        {  0, -1 },
        {  1,  0 },
        {  0,  1 },
        {  -1, 0 }
        };

    static const int Lturn[4] = { 3, 0, 1, 2 };
    static const int Rturn[4] = { 1, 2, 3, 0 };

    // variables
    int Grid[GridLen][GridWid];
    size_t n, i, d, j, k, inc;
    int viewx, viewy, x, y;
    char move, view, buf[64];
    double vf, minf;

    EvolvingFSM<char,ViewSz,char,MoveSz> * vm;

    RouletteWheel<double> * rw;

    RandDev devgen;

    // FSM_TranData<char> td[3][3];
    FSM_TranData<char> (* td)[MoveSz] = new FSM_TranData<char> [StatSz][MoveSz];

    if (td == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    EVFSM_MuteWts wts =
        {
        raoc.GetWeightO(),
        raoc.GetWeightT(),
        raoc.GetWeightA(),
        raoc.GetWeightD(),
        raoc.GetWeightN()
        };

    // allocate buffers
    EvolvingFSM<char,ViewSz,char,MoveSz> ** pop
        = new EvolvingFSM<char,ViewSz,char,MoveSz> * [POP_SZ];

    if (pop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    EvolvingFSM<char,ViewSz,char,MoveSz> ** newpop
        = new EvolvingFSM<char,ViewSz,char,MoveSz> * [POP_SZ];

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
    EvolvingFSM<char,ViewSz,char,MoveSz> * * ptrm = pop - 1;
    double * ptrf = fit - 1;

    // create an initial population of FSMs
    for (j = 0; j < POP_SZ; ++j)
        {
        for (n = 0; n < StatSz; ++n)
            {
            for (i = 0; i < ViewSz; ++i)
                {
                td[n][i].NextState = size_t(devgen()
                                       * float(StatSz));

                if (i == 0)
                    td[n][i].Osym = 'A';
                else
                    td[n][i].Osym = MoveSet[size_t(devgen()
                                         * float(MoveSz))];
                }
            }

        pop[j] = new EvolvingFSM<char,ViewSz,char,MoveSz>
                    (StatSz,&td[0][0],ViewSet,MoveSet,
                     size_t(devgen() * float(StatSz)),wts);

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
        wsprintf(buf,"Forge (loop: %u of %u)",
                     g,raoc.GetTestSize());

        SetWindowText(wdw,buf);

        minf = DBL_MAX;

        // fitness testing by examining the trail
        for (j = 0; j < POP_SZ; ++j)
            {
            // reset fsm
            pop[j]->Reset();
            fit[j] = 0.0;

            // initialize search grid
            memcpy(&(Grid[0][0]),
                   &(MasterGrid[0][0]),
                   sizeof(Grid));

            // start at beginning, in center, heading south
            y = 0;
            x = 5;
            d = 2;

            // do the moves
            for (k = 0; k < raoc.GetMaxMoves(); ++k)
                {
                // eat any food found in current location
                if (Grid[y][x])
                    {
                    fit[j] += 1.0;
                    Grid[y][x] = 0;
                    }

                // look ahead at next location
                viewx = x + Offset[d][0];
                viewy = y + Offset[d][1];

                // looking past end of grid -- done!
                if (viewy == GridLen)
                    break;

                // what does the ant see?
                if ((viewx < 0)
                ||  (viewx >= GridWid)
                ||  (viewy < 0))
                    {
                    view = 'C'; // cliff
                    }
                else
                    {
                    if (Grid[viewy][viewx])
                        view = 'F'; // food
                    else
                        view = 'E'; // empty
                    }

                // get move from state transition
                move = pop[j]->Transition(view);

                // apply move
                switch (move)
                    {
                    case 'L': // turn left
                        d = Lturn[d];
                        viewx = x + Offset[d][0];
                        viewy = y + Offset[d][1];
                        break;
                    case 'R': // turn right
                        d = Rturn[d];
                        viewx = x + Offset[d][0];
                        viewy = y + Offset[d][1];
                    }

                if ((viewx >= 0)
                &&  (viewx < GridWid)
                &&  (viewy >= 0))
                    {
                    x = viewx;
                    y = viewy;
                    }
                }

            if (fit[j] < minf)
                minf = fit[j];
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
        if (((g % raoc.GetReptFreq()) == 0)
          ||  (g == raoc.GetTestSize()))
            {
            buffer << "\r\nGeneration " << g
                   << "\r\n----------------\r\n";

            pop[0]->DumpStructure(buffer);

            buffer << "      Fitness: " << fit[0] << "\r\n";
            }

        // exit, if it's time
        if (g == raoc.GetTestSize())
            break;

        // fitness scaling (linear normalization)
        if (raoc.GetFitScale())
            {
            fit[0] = raoc.GetFitLinBase();
            i      = 1;

            while ((i < POP_SZ)
                    && (fit[i-1] <= raoc.GetFitLinDec()))
                {
                fit[i] = fit[i-1] - raoc.GetFitLinDec();
                ++i;
                }

            for (; i < POP_SZ; ++i)
                fit[i] = raoc.GetFitLinMin();
            }

        // elitist selection
        newpop[0] = pop[0];

        // reproduce new members
        rw = new RouletteWheel<double> (POP_SZ,fit);

        if (rw == NULL)
            {
            buffer << "Memory allocation failed\r\n";
            return;
            }

        for (j = 1; j < POP_SZ; ++j)
            {
            i = rw->GetIndex();

            newpop[j] = new EvolvingFSM<char,ViewSz,char,MoveSz>
                                             (*(pop[i]));

            if (newpop[j] == NULL)
                {
                buffer << "Memory allocation failed\r\n";
                return;
                }

            if (devgen() < raoc.GetMuteRate())
                newpop[j]->Mutate(2,raoc.GetMaxState());
            }

        delete rw;

        // copy next generation and repeat!
        for (j = 1; j < POP_SZ; ++j)
            delete pop[j];

        memcpy(pop,newpop,POP_SZ * sizeof(EvolvingFSM<char,ViewSz,char,MoveSz> *));

        ++g;
        }

    // delete buffers
    for (j = 0; j < POP_SZ; ++j)
         delete pop[j];

    delete [] pop;
    delete [] newpop;
    delete [] fit;
    delete [] td;

    // restore window text
    SetWindowText(wdw,"Forge");
    }

