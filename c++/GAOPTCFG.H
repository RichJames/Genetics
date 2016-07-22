//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      gaoptcfg.h     v1.00
//
//      A class for genetic algorithm optimization
//      configuration
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_GAOPTCFG_H
#define FORGE_GAOPTCFG_H

#include "bool.h"
#include "windows.h"

// genetic algorithm optimization configuration
enum FSAlgor
    {
    FSA_EXPON,
    FSA_WINDOW,
    FSA_LINEAR
    };

class GAOptConfig
    {
    public:
        GAOptConfig
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
        bool GetValidity() { return Valid; }

        // interrogate dimensions of test
        size_t GetPopSize()   { return PopSize;   }
        size_t GetTestSize()  { return TestSize;  }
        size_t GetReptFreq()  { return ReptFreq;  }
        size_t GetSigDigits() { return SigDigits; }

        // interrogate ranges of X and Y
        double GetXMin() { return XMin; }
        double GetXMax() { return XMax; }
        double GetYMin() { return YMin; }
        double GetYMax() { return YMax; }

        // interrogate crossover techniques
        float GetCrossRate() { return CrossRate; }
        bool  GetCrossX()    { return CrossX;    }
        bool  GetCrossY()    { return CrossY;    }
        bool  GetCrossB()    { return CrossB;    }

        // interrogate parameters for mutation
        float GetWtSign()   { return WtSign;   }
        float GetWtExp()    { return WtExp;    }
        float GetWtMant()   { return WtMant;   }
        float GetMuteRate() { return MuteRate; }
        bool  GetMutateX()  { return MutateX;  }
        bool  GetMutateY()  { return MutateY;  }
        bool  GetMuteLoop() { return MuteLoop; }

        // interrogate selection flag
        bool  GetElitist() { return Elitist; }

        // interrogate fitness scaling parameters
        bool    GetFitScale()   { return FitScale;   }
        FSAlgor GetFitAlgor()   { return FitAlgor;   }
        double  GetFitLinBase() { return FitLinBase; }
        double  GetFitLinDec()  { return FitLinDec;  }
        double  GetFitLinMin()  { return FitLinMin;  }

        // interrogate equation #
        size_t GetEquation() { return Equation; }

    private:
        // validity flag
        bool   Valid;

        // dimensions of test
        size_t PopSize;
        size_t TestSize;
        size_t ReptFreq;
        size_t SigDigits;

        // ranges of X and Y
        double XMin;
        double XMax;
        double YMin;
        double YMax;

        // crossover techniques
        float CrossRate;
        bool  CrossX;
        bool  CrossY;
        bool  CrossB;

        // parameters for mutation
        float WtSign;
        float WtExp;
        float WtMant;
        float MuteRate;
        bool  MutateX;
        bool  MutateY;
        bool  MuteLoop;

        // selection flag
        bool  Elitist;

        // fitness scaling
        bool    FitScale;
        FSAlgor FitAlgor;
        double  FitLinBase;
        double  FitLinDec;
        double  FitLinMin;

        // equation selection
        size_t  Equation;

        // stored parameter values
        static size_t  DefPopSize;
        static size_t  DefTestSize;
        static size_t  DefReptFreq;
        static size_t  DefSigDigits;
        static double  DefXMin;
        static double  DefXMax;
        static double  DefYMin;
        static double  DefYMax;
        static float   DefCrossRate;
        static bool    DefCrossX;
        static bool    DefCrossY;
        static bool    DefCrossB;
        static float   DefWtSign;
        static float   DefWtExp;
        static float   DefWtMant;
        static float   DefMuteRate;
        static bool    DefMutateX;
        static bool    DefMutateY;
        static bool    DefMuteLoop;
        static bool    DefElitist;
        static bool    DefFitScale;
        static FSAlgor DefFitAlgor;
        static double  DefFitLinBase;
        static double  DefFitLinDec;
        static double  DefFitLinMin;
        static size_t  DefEquation;
    };

#endif