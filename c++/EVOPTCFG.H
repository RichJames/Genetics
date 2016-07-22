//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      evoptcfg.h     v1.00
//
//      A class for configuring evolving state machines.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_EVOPTCFG_H
#define FORGE_EVOPTCFG_H

#include "bool.h"
#include "windows.h"

enum EVMove
    {
    EVM_DEFECT,
    EVM_COOPER,
    EVM_RANDOM
    };

class EVOptConfig
    {
    public:
        EVOptConfig
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
        size_t GetTestLen()    { return TestLen;    }
        size_t GetReptFreq()   { return ReptFreq;   }
        size_t GetMaxState()   { return MaxState;   }

        // interrogate operator parameters
        float  GetMuteRate()   { return MuteRate;   }
        float  GetWeightO()    { return WeightO;    }
        float  GetWeightT()    { return WeightT;    }
        float  GetWeightA()    { return WeightA;    }
        float  GetWeightD()    { return WeightD;    }
        float  GetWeightN()    { return WeightN;    }

        // interrogate payoff amounts
        double GetPayoffDD()   { return PayoffDD;   }
        double GetPayoffDC()   { return PayoffDC;   }
        double GetPayoffCD()   { return PayoffCD;   }
        double GetPayoffCC()   { return PayoffCC;   }

        // interrogate first move strategy
        EVMove GetFirstMove()  { return FirstMove;  }

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
        size_t  TestLen;
        size_t  ReptFreq;
        size_t  MaxState;

        // operator parameters
        float   MuteRate;
        float   WeightO;
        float   WeightT;
        float   WeightA;
        float   WeightD;
        float   WeightN;

        // payoff amounts
        double  PayoffDD;
        double  PayoffDC;
        double  PayoffCD;
        double  PayoffCC;

        // first move strategy
        EVMove  FirstMove;

        // fitness scaling parameters
        bool    FitScale;
        double  FitLinBase;
        double  FitLinDec;
        double  FitLinMin;

        // stored parameter values
        static size_t DefPopSize;
        static size_t DefTestSize;
        static size_t DefTestLen;
        static size_t DefReptFreq;
        static size_t DefMaxState;
        static float  DefMuteRate;
        static float  DefWeightO;
        static float  DefWeightT;
        static float  DefWeightA;
        static float  DefWeightD;
        static float  DefWeightN;
        static double DefPayoffDD;
        static double DefPayoffDC;
        static double DefPayoffCD;
        static double DefPayoffCC;
        static EVMove DefFirstMove;
        static bool   DefFitScale;
        static double DefFitLinBase;
        static double DefFitLinDec;
        static double DefFitLinMin;
    };

#endif
