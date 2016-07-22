//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      pdoptcfg.h     v1.00
//
//      A class for configuring Prisoner's Dilemma simulation
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_PDOPTCFG_H
#define FORGE_PDOPTCFG_H

#include "bool.h"
#include "windows.h"

class PDOptConfig
    {
    public:
        PDOptConfig
            (
            HINSTANCE inst,
            HWND      parent
            );

        static void DlgSetFileName
            (
            HWND  dlg
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
        size_t GetPopSize()  { return PopSize;  }
        size_t GetTestSize() { return TestSize; }
        size_t GetReptFreq() { return ReptFreq; }
        size_t GetStartLvl() { return StartLvl; }
        size_t GetMaxLevel() { return MaxLevel; }

        // interrogate operator parameters
        bool  GetCrossover() { return Crossover; }
        float GetCrossRate() { return CrossRate; }
        bool  GetMutation()  { return Mutation;  }
        float GetMuteRate()  { return MuteRate;  }
        bool  GetDoubling()  { return Doubling;  }
        float GetDblRate()   { return DblRate;   }
        bool  GetAveraging() { return Averaging; }
        bool  GetFairStart() { return FairStart; }
        bool  GetRandom1st() { return Random1st; }

        // interrogate payoff amounts
        double GetPayoffDD() { return PayoffDD; }
        double GetPayoffDC() { return PayoffDC; }
        double GetPayoffCD() { return PayoffCD; }
        double GetPayoffCC() { return PayoffCC; }

        // interrogate output file information
        bool         GetFileOut()  { return FileOut; }
        const char * GetFileName() { return FileName; }

    private:
        // validity flag
        bool   Valid;

        // dimensions of test
        size_t PopSize;
        size_t TestSize;
        size_t ReptFreq;
        size_t StartLvl;
        size_t MaxLevel;

        // interrogate operator parameters
        bool  Crossover;
        float CrossRate;
        bool  Mutation;
        float MuteRate;
        bool  Doubling;
        float DblRate;
        bool  Averaging;
        bool  FairStart;
        bool  Random1st;

        // payoff amounts
        double PayoffDD;
        double PayoffDC;
        double PayoffCD;
        double PayoffCC;

        // interrogate output file information
        bool FileOut;
        char FileName[256];

        // stored parameter values
        static size_t DefPopSize;
        static size_t DefTestSize;
        static size_t DefReptFreq;
        static size_t DefStartLvl;
        static size_t DefMaxLevel;
        static bool   DefCrossover;
        static float  DefCrossRate;
        static bool   DefMutation;
        static float  DefMuteRate;
        static bool   DefDoubling;
        static float  DefDblRate;
        static bool   DefAveraging;
        static bool   DefFairStart;
        static bool   DefRandom1st;
        static double DefPayoffDD;
        static double DefPayoffDC;
        static double DefPayoffCD;
        static double DefPayoffCC;
        static bool   DefFileOut;
        static char   DefFileName[256];
    };

#endif
