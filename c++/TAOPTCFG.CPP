//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      taoptcfg.cpp     v1.00
//
//      A class for configuring Travelling Artist
//      optimization.
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "forgeres.h"
#include "taoptcfg.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

size_t TAOptConfig::DefPopSize    =   101;
size_t TAOptConfig::DefTestSize   =   100;
size_t TAOptConfig::DefReptFreq   =     5;
size_t TAOptConfig::DefShowBest   =    10;
float  TAOptConfig::DefOperChance =     0.600F;
bool   TAOptConfig::DefMutation   =  true;
bool   TAOptConfig::DefInversion  = false;
bool   TAOptConfig::DefPMX        = false;
bool   TAOptConfig::DefCX         = false;
bool   TAOptConfig::DefOX         = false;
float  TAOptConfig::DefWeightM    =    10.0F;
float  TAOptConfig::DefWeightI    =    10.0F;
float  TAOptConfig::DefWeightP    =    10.0F;
float  TAOptConfig::DefWeightC    =    10.0F;
float  TAOptConfig::DefWeightO    =    10.0F;
double TAOptConfig::DefFitLinBase =   100.0;
double TAOptConfig::DefFitLinDec  =    10.0;
double TAOptConfig::DefFitLinMin  =     1.0;

static LPCSTR TAOptConfigDlgName = MAKEINTRESOURCE(DLG_TAOPTCFG);

static const char * TAOptConfigDlgErr =
    "Population and Generations must be >= 10.\n"
    "Population must be an ODD number.\n"
    "Operation Probability must be <= 100.0\n"
    "Show Best must be greater than 0 and <= population.\n"
    "Sum of Operation weights must be > 1.\n"
    "Report Frequency must be > 0 and < generations.";

BOOL CALLBACK TAOptConfigDlgProc
    (
    HWND  dlg,
    WORD  message,
    WORD  wParam,
    DWORD lParam
    );

TAOptConfig::TAOptConfig
    (
    HINSTANCE inst,
    HWND      parent
    )
    {
    int res;

    #ifdef _WIN32
        res = DialogBox(inst,TAOptConfigDlgName,parent,FARPROC(TAOptConfigDlgProc));
    #else
        DLGPROC thunk   = MakeProcInstance(FARPROC(TAOptConfigDlgProc),inst);
        res = DialogBox(inst,TAOptConfigDlgName,parent,thunk);
        FreeProcInstance(thunk);
    #endif

    if (res)
        Valid = true;
    else
        Valid = false;

    PopSize    = DefPopSize;
    TestSize   = DefTestSize;
    ReptFreq   = DefReptFreq;
    ShowBest   = DefShowBest;
    OperChance = DefOperChance;
    Mutation   = DefMutation;
    Inversion  = DefInversion;
    PMX        = DefPMX;
    CX         = DefCX;
    OX         = DefOX;
    WeightM    = DefWeightM;
    WeightI    = DefWeightI;
    WeightP    = DefWeightP;
    WeightC    = DefWeightC;
    WeightO    = DefWeightO;
    FitLinBase = DefFitLinBase;
    FitLinDec  = DefFitLinDec;
    FitLinMin  = DefFitLinMin;
    }
#ifdef __BORLANDC__
    #pragma argsused
#endif

BOOL CALLBACK TAOptConfigDlgProc
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
            TAOptConfig::DlgInit(dlg);
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
                {
                case IDOK:
                    if (TRUE == TAOptConfig::DlgStore(dlg))
                        EndDialog(dlg, 1);

                    break;

                case IDCANCEL:
                    EndDialog(dlg, 0);
                }

            return TRUE;
        }

    return FALSE;
    }

void TAOptConfig::DlgInit
    (
    HWND  dlg
    )
    {
    char temp[64];

    sprintf(temp,"%u",DefPopSize);
    SetDlgItemText(dlg,IDD_TAOPTCFG_POP,temp);

    sprintf(temp,"%u",DefTestSize);
    SetDlgItemText(dlg,IDD_TAOPTCFG_GEN,temp);

    sprintf(temp,"%u",DefReptFreq);
    SetDlgItemText(dlg,IDD_TAOPTCFG_FREQ,temp);

    sprintf(temp,"%u",DefShowBest);
    SetDlgItemText(dlg,IDD_TAOPTCFG_SHOW,temp);

    sprintf(temp,"%.6g",DefOperChance * 100.0);
    SetDlgItemText(dlg,IDD_TAOPTCFG_PROB,temp);

    if (DefMutation)
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_MUTE,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_MUTE,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefWeightM);
    SetDlgItemText(dlg,IDD_TAOPTCFG_WTM,temp);

    if (DefInversion)
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_INV,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_INV,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefWeightI);
    SetDlgItemText(dlg,IDD_TAOPTCFG_WTI,temp);

    if (DefPMX)
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_PMX,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_PMX,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefWeightP);
    SetDlgItemText(dlg,IDD_TAOPTCFG_WTP,temp);

    if (DefCX)
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_CX,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_CX,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefWeightC);
    SetDlgItemText(dlg,IDD_TAOPTCFG_WTC,temp);

    if (DefOX)
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_OX,BM_SETCHECK,1,0L);
    else
        SendDlgItemMessage(dlg,IDD_TAOPTCFG_OX,BM_SETCHECK,0,0L);

    sprintf(temp,"%.6g",DefWeightO);
    SetDlgItemText(dlg,IDD_TAOPTCFG_WTO,temp);

    sprintf(temp,"%.15g",DefFitLinBase);
    SetDlgItemText(dlg,IDD_TAOPTCFG_LNBASE,temp);

    sprintf(temp,"%.15g",DefFitLinDec);
    SetDlgItemText(dlg,IDD_TAOPTCFG_LNDEC,temp);

    sprintf(temp,"%.15g",DefFitLinMin);
    SetDlgItemText(dlg,IDD_TAOPTCFG_LNMIN,temp);
    }

BOOL TAOptConfig::DlgStore
    (
    HWND  dlg
    )
    {
    char temp[64];

    GetDlgItemText(dlg,IDD_TAOPTCFG_POP,temp,64);
    size_t psz = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_TAOPTCFG_GEN,temp,64);
    size_t gen = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_TAOPTCFG_FREQ,temp,64);
    size_t frq = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_TAOPTCFG_SHOW,temp,64);
    size_t shw = abs(atoi(temp));

    GetDlgItemText(dlg,IDD_TAOPTCFG_PROB,temp,64);
    float prb = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_WTM,temp,64);
    float wtm = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_WTI,temp,64);
    float wti = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_WTP,temp,64);
    float wtp = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_WTC,temp,64);
    float wtc = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_WTO,temp,64);
    float wto = float(fabs(atof(temp)));

    if ((psz < 10)
    ||  (!(psz & 1))
    ||  (gen < 10)
    ||  (frq <  1)
    ||  (frq > gen)
    ||  (prb > 100.0F)
    ||  ((wti + wtm + wtp + wtc + wto) < 1.0F)
    ||  (shw < 1)
    ||  (shw > psz))
        {
        MessageBeep(MB_ICONHAND);
        MessageBox(NULL,TAOptConfigDlgErr,"Configuration Error",MB_OK | MB_ICONHAND);
        return FALSE;
        }

    DefPopSize    = psz;
    DefTestSize   = gen;
    DefReptFreq   = frq;
    DefShowBest   = shw;
    DefOperChance = prb / 100.0F;
    DefWeightM    = wtm;
    DefWeightI    = wti;
    DefWeightP    = wtp;
    DefWeightC    = wtc;
    DefWeightO    = wto;

    if (SendDlgItemMessage(dlg,IDD_TAOPTCFG_MUTE,BM_GETCHECK,0,0L))
        DefMutation = true;
    else
        DefMutation = false;

    if (SendDlgItemMessage(dlg,IDD_TAOPTCFG_INV,BM_GETCHECK,0,0L))
        DefInversion = true;
    else
        DefInversion = false;

    if (SendDlgItemMessage(dlg,IDD_TAOPTCFG_PMX,BM_GETCHECK,0,0L))
        DefPMX = true;
    else
        DefPMX = false;

    if (SendDlgItemMessage(dlg,IDD_TAOPTCFG_CX,BM_GETCHECK,0,0L))
        DefCX = true;
    else
        DefCX = false;

    if (SendDlgItemMessage(dlg,IDD_TAOPTCFG_OX,BM_GETCHECK,0,0L))
        DefOX = true;
    else
        DefOX = false;

    GetDlgItemText(dlg,IDD_TAOPTCFG_LNBASE,temp,64);
    DefFitLinBase = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_LNDEC,temp,64);
    DefFitLinDec = float(fabs(atof(temp)));

    GetDlgItemText(dlg,IDD_TAOPTCFG_LNMIN,temp,64);
    DefFitLinMin = float(fabs(atof(temp)));

    return TRUE;
    }

