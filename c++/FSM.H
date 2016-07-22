//-----------------------------------------------------------
//  Finite State Machines
//-----------------------------------------------------------
//
//      fsm.h           v1.00
//
//      A template defining finite state machine types.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FSM_H
#define FSM_H

#include "diagnose.h"
#include "bool.h"
#include "string.h"
#include "roulette.h"
#include "iostream.h"
#include "iomanip.h"

//----------------
// FSM Error types
//----------------

class FSM_Ex : public ExceptionBase
    {
    public:
		virtual void Explain
            (
            DiagOutput & out
            );
    };

//----------------------
// state transition data
//----------------------

template
    <
    class Tout
    >
    struct FSM_TranData
        {
        size_t NextState;
        Tout  Osym;

        // default constructor
        FSM_TranData() : NextState(0), Osym() { }

        // constructor
        FSM_TranData
            (
            size_t ns,
            Tout  os
            )
            {
            NextState = ns;
            Osym = os;
            }
        };

//--------------------------------
// Core finite state machine class
//--------------------------------

template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    class FiniteStateMachine
        {
        public:
            // origination constructor
            FiniteStateMachine
                (
                size_t nState,
                FSM_TranData<Tout> * table,
                Tin  iset[Nin],
                size_t start
                );

            // destructor
            ~FiniteStateMachine();

            // copy constructor
            FiniteStateMachine
                (
                const FiniteStateMachine<Tin,Nin,Tout> & fsm
                );

            // assignment operator
            void operator =
                (
                const FiniteStateMachine<Tin,Nin,Tout> & fsm
                );

            // interrogators
            size_t GetNumStates() { return N;     }
            size_t GetCurState()  { return State; }

            // set to next state based on input
            Tout Transition
                (
                Tin in
                );

            // reset machine to initial state
            void Reset();

            // dump a description of this machine
            void DumpStructure
                (
                ostream & str
                );

        protected:
            FSM_TranData<Tout> (* TranTable)[Nin];
            Tin    InSymSet[Nin];
            size_t N;
            size_t State;
            size_t InitState;
        };

// origination constructor
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    FiniteStateMachine<Tin,Nin,Tout>::FiniteStateMachine
        (
        size_t nState,
        FSM_TranData<Tout> * table,
        Tin  iset[Nin],
        size_t start
        )
        {
        if (start >= nState)
            throw FSM_Ex();

        N = nState;

        TranTable = new FSM_TranData<Tout>[N][Nin];

        if (TranTable == NULL) throw FSM_Ex();

        memcpy(TranTable,table,
               N * Nin * sizeof(FSM_TranData<Tout>));

        memcpy(InSymSet,iset,Nin * sizeof(Tin));

        State     = start;
        InitState = start;
        }

// destructor
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    FiniteStateMachine<Tin,Nin,Tout>::~FiniteStateMachine()
        {
        if (TranTable != NULL)
            delete [] TranTable;
        }

// assignment operator
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    void FiniteStateMachine<Tin,Nin,Tout>::operator =
        (
        const FiniteStateMachine<Tin,Nin,Tout> & fsm
        )
        {
        if (N != fsm.N)
            {
            N = fsm.N;

            if (TranTable != NULL)
                {
                delete [] TranTable;
                TranTable = NULL;
                }
            }

        if (TranTable == NULL)
            {
            TranTable = new FSM_TranData<Tout>[N][Nin];

            if (TranTable == NULL)
                throw FSM_Ex();
            }

        memcpy(TranTable,fsm.TranTable,
               N * Nin * sizeof(FSM_TranData<Tout>));

        memcpy(InSymSet,fsm.InSymSet,Nin * sizeof(Tin));

        State     = fsm.State;
        InitState = fsm.InitState;
        }

// copy constructor
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    FiniteStateMachine<Tin,Nin,Tout>::FiniteStateMachine
        (
        const FiniteStateMachine<Tin,Nin,Tout> & fsm
        )
        {
        TranTable = NULL;
        (*this) = fsm;
        }

// set to next state based on input
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    Tout FiniteStateMachine<Tin,Nin,Tout>::Transition
        (
        Tin in
        )
        {
        if (TranTable == NULL)
            throw FSM_Ex();

        size_t i = 0;

        while ((i < Nin) && (InSymSet[i] != in)) ++i;

        if (i == Nin)
            throw FSM_Ex();

        Tout res = TranTable[State][i].Osym;

        State = TranTable[State][i].NextState;

        if (State >= N)
            throw FSM_Ex();

        return res;
        }

// reset machine to initial state
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    inline void FiniteStateMachine<Tin,Nin,Tout>::Reset()
        {
        State = InitState;
        }

// dump a description of this machine
template
    <
    class  Tin,
    size_t Nin,
    class  Tout
    >
    void FiniteStateMachine<Tin,Nin,Tout>::DumpStructure
        (
        ostream & str
        )
        {
        str << "Initial state: " << InitState << "\r\n";
        str << "Current state: " << State     << "\r\n";
        str << " Total states: " << N         << "\r\n";

        for (size_t s = 0; s < N; ++s)
            {
            str << "s: " << s;

            for (size_t i = 0; i < Nin; ++i)
                {
                str << ", " << InSymSet[i]
                    << "="  << TranTable[s][i].Osym
                    << ">>" << TranTable[s][i].NextState;
                }

            str << "\r\n";
            }
        }

//---------------------------------
// Genetic Algorithm-specific types
//---------------------------------

typedef float EVFSM_MuteWts[5];

enum EVFSM_MuteType
    {
    EVFSM_OutSymbol = 0,
    EVFSM_Transition,
    EVFSM_AddState,
    EVFSM_DelState,
    EVFSM_InitState,
    EVFSM_None
    };

template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    class EvolvingFSM : public FiniteStateMachine<Tin,Nin,Tout>
        {
        public:
            // default constructor
            EvolvingFSM();

            // origination constructor
            EvolvingFSM
                (
                size_t nState,
                FSM_TranData<Tout> * table,
                Tin  iset[Nin],
                Tout oset[Nout],
                size_t start,
                EVFSM_MuteWts wts
                );

            // copy constructor
            EvolvingFSM
                (
                const EvolvingFSM<Tin,Nin,Tout,Nout> & fsm
                );

            // assignment operator
            void operator =
                (
                const EvolvingFSM<Tin,Nin,Tout,Nout> & fsm
                );

            // mutation
            EVFSM_MuteType Mutate
                (
                size_t minstate,
                size_t maxstate
                );

        protected:
            Tout OutSymSet[Nout];
            RouletteWheel<float> MuteChooser;
            RandDev DevGen;

            size_t PickIsym();
            size_t PickOsym();
            size_t PickState();
        };

template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    inline size_t EvolvingFSM<Tin,Nin,Tout,Nout>::PickIsym()
        {
        return size_t(DevGen() * float(Nin));
        }

template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    inline size_t EvolvingFSM<Tin,Nin,Tout,Nout>::PickOsym()
        {
        return size_t(DevGen() * float(Nout));
        }

template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    inline size_t EvolvingFSM<Tin,Nin,Tout,Nout>::PickState()
        {
        return size_t(DevGen() * float(N));
        }

// origination constructor
template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    EvolvingFSM<Tin,Nin,Tout,Nout>::EvolvingFSM
        (
        size_t nState,
        FSM_TranData<Tout> * table,
        Tin  iset[Nin],
        Tout oset[Nout],
        size_t start,
        EVFSM_MuteWts wts
        )
        : FiniteStateMachine<Tin,Nin,Tout>(nState,table,iset,start),
          MuteChooser(5,wts)
        {
        memcpy(OutSymSet,oset,Nout * sizeof(Tout));
        }

// copy constructor
template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    EvolvingFSM<Tin,Nin,Tout,Nout>::EvolvingFSM
        (
        const EvolvingFSM<Tin,Nin,Tout,Nout> & fsm
        )
        : FiniteStateMachine<Tin,Nin,Tout>(fsm),
          MuteChooser(fsm.MuteChooser)
        {
        memcpy(OutSymSet,fsm.OutSymSet,Nout * sizeof(Tout));
        }

// assignment operator
template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    void EvolvingFSM<Tin,Nin,Tout,Nout>::operator =
        (
        const EvolvingFSM<Tin,Nin,Tout,Nout> & fsm
        )
        {
        FiniteStateMachine<Tin,Nin,Tout>::operator = (fsm);

        MuteChooser = fsm.MuteChooser;

        memcpy(OutSymSet,fsm.OutSymSet,Nin * sizeof(Tin));
        }

// reproduce
template
    <
    class  Tin,
    size_t Nin,
    class  Tout,
    size_t Nout
    >
    EVFSM_MuteType EvolvingFSM<Tin,Nin,Tout,Nout>::Mutate
        (
        size_t minstate,
        size_t maxstate
        )
        {
        // correct and verify parameters
        if (minstate < 1)
            minstate = 1;

        if ((TranTable == NULL) || (minstate > maxstate))
            throw FSM_Ex();

        // define variables
        size_t n, i, j, s;
        EVFSM_MuteType c;
        FSM_TranData<Tout> (* newtable)[Nin];

        // select operation
        c = EVFSM_MuteType(MuteChooser.GetIndex());

        // skip invalid cases
        if ((c == EVFSM_AddState) && (N >= maxstate))
            return EVFSM_None;

        if ((c == EVFSM_DelState) && (N <= minstate))
            return EVFSM_None;

        if ((c == EVFSM_InitState) && (N <= 1))
            return EVFSM_None;

        // perform operation
        switch (c)
            {
            case EVFSM_OutSymbol:
                // pick state to modify
                n = PickState();

                // pick input symbol to modify
                i = PickIsym();

                // change output symbol
                TranTable[n][i].Osym = OutSymSet[PickOsym()];

                break;

            case EVFSM_Transition:
                // pick state to modify
                n = PickState();

                // pick input state to modify
                i = PickIsym();

                // pick new output state
                s = PickState();

                // change state transition
                TranTable[n][i].NextState = s;
                break;

            case EVFSM_AddState:
                newtable = new FSM_TranData<Tout>[N+1][Nin];

                if (newtable == NULL)
                    throw FSM_Ex();

                // copy old states
                for (n = 0; n < N; ++n)
                    memcpy(newtable[n],TranTable[n],Nin * sizeof(FSM_TranData<Tin>));

                // fill new state with random transition data
                for (i = 0; i < Nin; ++i)
                    {
                    newtable[N][i].Osym = OutSymSet[PickOsym()];
                    newtable[N][i].NextState = size_t(DevGen() * float(N + 1.0F));
                    }

                // change some old states to point to new state
                i = size_t(DevGen() * N / 2) + 1;

                for (n = 0; n < i; ++n)
                    newtable[PickState()][PickIsym()].NextState = N;

                // increment number of states
                ++N;

                // use new table, delete the old one
                delete [] TranTable;

                TranTable = newtable;

                break;

            case EVFSM_DelState:
                if (N == 1) break;

                // create new transition table
                newtable = new FSM_TranData<Tout>[N-1][Nin];

                if (newtable == NULL)
                    throw FSM_Ex();

                // choose state to be deleted
                n = PickState();

                // copy old states
                i = 0;

                for (s = 0; s < N; ++s)
                    {
                    if (s != n)
                        {
                        // copy state information
                        memcpy(newtable[i],TranTable[s],
                               Nin * sizeof(FSM_TranData<Tin>));

                        // change references to deleted state
                        for (j = 0; j < Nin; ++j)
                            {
                            if (newtable[i][j].NextState == n)
                                {
                                newtable[i][j].NextState =
                                        size_t(DevGen()
                                                 * float(N - 1));
                                }
                            else
                                {
                                if (newtable[i][j].NextState > n)
                                    --newtable[i][j].NextState;
                                }
                            }

                        ++i;
                        }
                    }

                // decrement number of states
                --N;

                // use new table, delete the old one
                delete [] TranTable;
                TranTable = newtable;

                // change initial state, if need be
                if (InitState == n)
                    InitState = PickState();
                else
                    if (InitState > n)
                        --InitState;

                break;

            case EVFSM_InitState:
                do  {
                    i = PickState();
                    }
                while (i == InitState);

                InitState = i;

                break;
            }

        Reset();

        return c;
        }

#endif
