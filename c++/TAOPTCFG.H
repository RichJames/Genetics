//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      taoptcfg.h     v1.00
//
//      A class for configuring Travelling Artist
//      optimization.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_TAOPTCFG_H
#define FORGE_TAOPTCFG_H

#include "bool.h"
#include "windows.h"

class TAOptConfig
    {
    public:
        TAOptConfig
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

        // verify if this object is valid
        bool   GetValidity()   { return Valid; }

        // interrogate dimensions of test
        size_t GetPopSize()    { return PopSize;    }
        size_t GetTestSize()   { return TestSize;   }
        size_t GetReptFreq()   { return ReptFreq;   }
        size_t GetShowBest()   { return ShowBest;   }

        // interrogate operator parameters
        float  GetOperChance() { return OperChance; }
        bool   GetMutation()   { return Mutation;   }
        bool   GetInversion()  { return Inversion;  }
        bool   GetPMX()        { return PMX;        }
        bool   GetCX()         { return CX;         }
        bool   GetOX()         { return OX;         }
        float  GetWeightM()    { return WeightM;    }
        float  GetWeightI()    { return WeightI;    }
        float  GetWeightP()    { return WeightP;    }
        float  GetWeightC()    { return WeightC;    }
        float  GetWeightO()    { return WeightO;    }

        // interrogate fitness scaling parameters
        double GetFitLinBase() { return FitLinBase; }
        double GetFitLinDec()  { return FitLinDec;  }
        double GetFitLinMin()  { return FitLinMin;  }

    private:
        // validity flag
        bool   Valid;

        // dimensions of test
        size_t PopSize;
        size_t TestSize;
        size_t ReptFreq;
        size_t ShowBest;

        // operator parameters
        float  OperChance;
        bool   Mutation;
        bool   Inversion;
        bool   PMX;
        bool   CX;
        bool   OX;
        float  WeightM;
        float  WeightI;
        float  WeightP;
        float  WeightC;
        float  WeightO;

        // fitness scaling parameters
        double  FitLinBase;
        double  FitLinDec;
        double  FitLinMin;

        // stored parameter values
        static size_t DefPopSize;
        static size_t DefTestSize;
        static size_t DefReptFreq;
        static size_t DefShowBest;
        static float  DefOperChance;
        static bool   DefMutation;
        static bool   DefInversion;
        static bool   DefPMX;
        static bool   DefCX;
        static bool   DefOX;
        static float  DefWeightM;
        static float  DefWeightI;
        static float  DefWeightP;
        static float  DefWeightC;
        static float  DefWeightO;
        static double DefFitLinBase;
        static double DefFitLinDec;
        static double DefFitLinMin;
    };

#endif
