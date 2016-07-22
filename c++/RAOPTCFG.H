//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      raoptcfg.h     v1.00
//
//      A class for configuring evolving robotic ants.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_RAOPTCFG_H
#define FORGE_RAOPTCFG_H

#include "bool.h"
#include "windows.h"

class RAOptConfig
    {
    public:
        RAOptConfig
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
        float  GetValidity()   { return Valid; }

        // interrogate dimensions of test
        size_t GetPopSize()    { return PopSize;    }
        size_t GetTestSize()   { return TestSize;   }
        size_t GetReptFreq()   { return ReptFreq;   }
        size_t GetMinState()   { return MinState;   }
        size_t GetMaxState()   { return MaxState;   }
        size_t GetMaxMoves()   { return MaxMoves;   }

        // interrogate operator parameters
        float  GetMuteRate()   { return MuteRate;   }
        float  GetWeightO()    { return WeightO;    }
        float  GetWeightT()    { return WeightT;    }
        float  GetWeightA()    { return WeightA;    }
        float  GetWeightD()    { return WeightD;    }
        float  GetWeightN()    { return WeightN;    }

        // interrogate fitness scaling parameters
        bool   GetFitScale()   { return FitScale;   }
        double GetFitLinBase() { return FitLinBase; }
        double GetFitLinDec()  { return FitLinDec;  }
        double GetFitLinMin()  { return FitLinMin;  }

    private:
        // validity flag
        float   Valid;

        // dimensions of test
        size_t  PopSize;
        size_t  TestSize;
        size_t  ReptFreq;
        size_t  MinState;
        size_t  MaxState;
        size_t  MaxMoves;

        // operator parameters
        float   MuteRate;
        float   WeightO;
        float   WeightT;
        float   WeightA;
        float   WeightD;
        float   WeightN;

        // fitness scaling parameters
        bool    FitScale;
        double  FitLinBase;
        double  FitLinDec;
        double  FitLinMin;

        // stored parameter values
        static size_t DefPopSize;
        static size_t DefTestSize;
        static size_t DefReptFreq;
        static size_t DefMinState;
        static size_t DefMaxState;
        static size_t DefMaxMoves;
        static float  DefMuteRate;
        static float  DefWeightO;
        static float  DefWeightT;
        static float  DefWeightA;
        static float  DefWeightD;
        static float  DefWeightN;
        static bool   DefFitScale;
        static double DefFitLinBase;
        static double DefFitLinDec;
        static double DefFitLinMin;
    };

#endif
