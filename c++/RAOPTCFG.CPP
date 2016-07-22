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

#include "forgeres.h"
#include "raoptcfg.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

size_t RAOptConfig::DefPopSize    =  50;
size_t RAOptConfig::DefTestSize   = 100;
size_t RAOptConfig::DefReptFreq   =   1;
size_t RAOptConfig::DefMinState   =   3;
size_t RAOptConfig::DefMaxState   =   5;
size_t RAOptConfig::DefMaxMoves   = 100;
float  RAOptConfig::DefMuteRate   =   0.5F;
float  RAOptConfig::DefWeightO    =  10.0F;
float  RAOptConfig::DefWeightT    =  10.0F;
float  RAOptConfig::DefWeightA    =  10.0F;
float  RAOptConfig::DefWeightD    =  10.0F;
float  RAOptConfig::DefWeightN    =  10.0F;
bool   RAOptConfig::DefFitScale   =  true;
double RAOptConfig::DefFitLinBase = 100.0;
double RAOptConfig::DefFitLinDec  =   5.0;
double RAOptConfig::DefFitLinMin  =   1.0;

static LPCSTR RAOptConfigDlgName = MAKEINTRESOURCE(DLG_RAOPTCFG);

static const char * RAOptConfigDlgErr =
    "Population and Generations must be >= 10.\n"
    "Report frequency must be > 0.\n"
    "MuteRate Rate must be <= 100.0\n"
    "Max and Min States must be between 2 and 16.\n"
    "Max State must be >= Min State.\n"
    "Max Moves must be >= 10.\n"
    "Sum of mutation weights must be > 1.";

BOOL CALLBACK RAOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

RAOptConfig::RAOptConfig
    (
    HINSTANCE inst,
    HWND      parent
    )
    {
    int res;

    #ifdef _WIN32
        res = DialogBox(inst,RAOptConfigDlgName,parent,FARPROC(RAOptConfigDlgProc));
    #else
        DLGPROC thunk   = MakeProcInstance(FARPROC(RAOptConfigDlgProc),inst);
        res = DialogBox(inst,RAOptConfigDlgName,parent,thunk);
        FreeProcInstance(thunk);
    #endif

    if (res)
        Valid = true;
    else
        Valid = false;

    PopSize    = DefPopSize;
    TestSize   = DefTestSize;
    ReptFreq   = DefReptFreq;
    MinState   = DefMinState;
    MaxState   = DefMaxState;
    MaxMoves   = DefMaxMoves;
    MuteRate   = DefMuteRate;
    WeightO    = DefWeightO;
    WeightT    = DefWeightT;
    WeightA    = DefWeightA;
    WeightD    = DefWeightD;
    WeightN    = DefWeightN;
    FitScale   = DefFitScale;
    FitLinBase = DefFitLinBase;
    FitLinDec  = DefFitLinDec;
    FitLinMin  = DefFitLinMin;
    }

#ifdef __BORLANDC__
#pragma argsused
#endif

BOOL CALLBACK RAOptConfigDlgProc
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
            RAOptConfig::DlgInit(dlg);
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
                {
                case IDOK:
                    if (TRUE == RAOptConfig::DlgStore(dlg))
                        EndDialog(dlg, 1);

                    break;

                case IDCANCEL:
                    EndDialog(dlg, 0);
                }

            return TRUE;
        }

    return FALSE;
    }

void RAOptConfig::DlgInit
    (
    HWND  dlg
    )
    {
    char temp[64];

    sprintf(temp,"%u",DefPopSize);
    SetDlgItemText(dlg,IDD_RAOPTCFG_POP,temp);

    sprintf(temp,"%u",DefTestSize);
    SetDlgItemText(dlg,IDD_RAOPTCFG_GEN,temp);

    sprintf(temp,"%u",DefReptFreq);
    SetDlgItemText(dlg,IDD_RAOPTCFG_FREQ,temp);

    sprintf(temp,"%u",DefMinState);
    SetDlgItemText(dlg,IDD_RAOPTCFG_MINSTATE,temp);

    sprintf(temp,"%u",DefMaxState);
    SetDlgItemText(dlg,IDD_RAOPTCFG_MAXSTATE,temp);

    sprintf(temp,"%u",DefMaxMoves);
    SetDlgItemText(dlg,IDD_RAOPTCFG_MAXMOVES,temp);

    sprintf(temp,"%.6g",DefMuteRate * 100.0);
    SetDlgItemText(dlg,IDD_RAOPTCFG_MRATE,temp);

    sprintf(temp,"%.6g",DefWeightO);
    SetDlgItemText(dlg,IDD_RAOPTCFG_WTO,temp);

    sprintf(temp,"%.6g",DefWeightT);
    SetDlgItemText(dlg,IDD_RAOPTCFG_WTT,temp);

    sprintf(temp,"%.6g",DefWeightA);
    SetDlgItemText(dlg,IDD_RAOPTCFG_WTA,temp);

    sprintf(temp,"%.6g",DefWeightD);
    SetDlgItemText(dlg,IDD_RAOPTCFG_WTD,temp);

    sprintf(temp,"%.6g",DefWeightN);
    SetDlgItemText(dlg,IDD_RAOPTCFG_WTN,temp);

    if (DefFitScale)
        SendDlgItemMessage(dlg,IDD_RAOPTCFG_FSCALE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_RAOPTCFG_FSCALE,BM_SETCHECK,0,0L);

    sprintf(temp,"%.15g",DefFitLinBase);
    SetDlgItemText(dlg,IDD_RAOPTCFG_LNBASE,temp);

    sprintf(temp,"%.15g",DefFitLinDec);
    SetDlgItemText(dlg,IDD_RAOPTCFG_LNDEC,temp);

    sprintf(temp,"%.15g",DefFitLinMin);
    SetDlgItemText(dlg,IDD_RAOPTCFG_LNMIN,temp);
    }

BOOL RAOptConfig::DlgStore
    (
    HWND  dlg
    )
    {
    char temp[64];

    GetDlgItemText(dlg,IDD_RAOPTCFG_POP,temp,64);
    size_t psz = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_RAOPTCFG_GEN,temp,64);
    size_t gen = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_RAOPTCFG_FREQ,temp,64);
    size_t frq = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_RAOPTCFG_MINSTATE,temp,64);
    size_t minst = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_RAOPTCFG_MAXSTATE,temp,64);
    size_t maxst = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_RAOPTCFG_MAXMOVES,temp,64);
    size_t maxmv = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_RAOPTCFG_MRATE,temp,64);
    float mrate = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_WTO,temp,64);
    float wto = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_WTT,temp,64);
    float wtt = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_WTA,temp,64);
    float wta = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_WTD,temp,64);
    float wtd = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_WTN,temp,64);
    float wtn = float(fabs(atof(temp)));

    if ((psz < 10)
    ||  (gen < 10)
    ||  (frq <  1)
    ||  (frq > gen)
    ||  (maxst <  2)
    ||  (maxst > 16)
    ||  (maxst < minst)
    ||  (minst <  2)
    ||  (maxmv < 10)
    ||  (mrate > 100.0F)
    ||  ((wto + wtt + wta + wtd + wtn) < 1.0F))
        {
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL,RAOptConfigDlgErr,"Configuration Error",MB_OK | MB_ICONHAND);
        return FALSE;
        }

    DefPopSize  = psz;
    DefTestSize = gen;
    DefReptFreq = frq;
    DefMinState = minst;
    DefMaxState = maxst;
    DefMaxMoves = maxmv;
    DefMuteRate = mrate / 100.0F;
    DefWeightO  = wto;
    DefWeightT  = wtt;
    DefWeightA  = wta;
    DefWeightD  = wtd;
    DefWeightN  = wtn;

    if (SendDlgItemMessage(dlg,IDD_RAOPTCFG_FSCALE,BM_GETCHECK,0,0L))
        DefFitScale = true;
    else
        DefFitScale = false;

    GetDlgItemText(dlg,IDD_RAOPTCFG_LNBASE,temp,64);
    DefFitLinBase = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_LNDEC,temp,64);
    DefFitLinDec = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_RAOPTCFG_LNMIN,temp,64);
    DefFitLinMin = float(fabs(atof(temp)));

    return TRUE;
    }



