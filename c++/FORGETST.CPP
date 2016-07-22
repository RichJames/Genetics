//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgetst.cpp     v1.00
//
//      Component tests
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#include "windows.h"  // Windows definitions
#include "strstrea.h" // strstream definitions
#include "roulette.h" // RouletteWheel class
#include "bool.h"     // ANSI-like 'bool' class
#include "bintreek.h" // keyed binary tree class
#include "sorts.h"    // small sorting template functions
#include "gafloat.h"  // mutation and crossover for IEEE floats
#include "fsm.h"      // finite state machine classes
#include "math.h"     // math functions
#include "float.h"    // floating point parameters
#include "fpconst.h"  // floating point constants
#include "fputil.h"   // floating-point utility functions

//--------------------------------------------
// Test RouletteWheel class
//--------------------------------------------

void TestRoulette
    (
    strstream & buffer
    )
    {
    buffer << "Test Roulette Wheel\r\n"
              "-------------------\r\n\r\n";

    float  rinit1[6] = { 10.0F, 600.0F, 50.0F, 50.0F, 190.0F, 100.0F };
    size_t count1[6] = { 0, 0, 0, 0, 0, 0 };
    size_t i;

    RouletteWheel<float> wheel(6,rinit1);

    // test wheel
    for (i = 0; i < 1000; ++i)
        ++count1[wheel.GetIndex()];

    // display counts
    for (i = 0; i < 6; ++i)
        {
        buffer << setw(6) << rinit1[i] << " hits: "
               << setw(4) << count1[i] << "\r\n";

        count1[i] = 0; // clear for next test
        }

    buffer << "\r\n";

    // now change a couple of values
    wheel.Change(1,100.0F);
    wheel.Change(5,300.0F);
    wheel.Change(0,310.0F);

    // test wheel
    for (i = 0; i < 1000; ++i)
        ++count1[wheel.GetIndex()];

    // display counts
    for (i = 0; i < 6; ++i)
        {
        buffer << setw(6) << wheel.GetWeight(i) << " hits: "
               << setw(4) << count1[i] << "\r\n";

        count1[i] = 0;
        }

    buffer << "\r\n";

    // test copy construction
    RouletteWheel<float> wheel2(wheel);

    // test wheel
    for (i = 0; i < 1000; ++i)
        ++count1[wheel.GetIndex()];

    // display counts
    for (i = 0; i < 6; ++i)
        {
        buffer << setw(6) << wheel.GetWeight(i) << " hits: "
               << setw(4) << count1[i] << "\r\n";

        count1[i] = 0;
        }
    }

//--------------------------------------------
// Test ANSI-like "bool" class
//--------------------------------------------

void TestBool
    (
    strstream & buffer
    )
    {
    buffer << "Test ANSI 'bool' class\r\n"
              "----------------------\r\n\r\n";

    bool b1;
    bool b2(9);
    bool b3 = true;

    buffer << "b1 = " << b1 << " (should be false)\r\n";
    buffer << "b2 = " << b2 << " (should be true)\r\n";
    buffer << "b3 = " << b3 << " (should be true)\r\n\r\n";

    b1 = b3;
    buffer << "after b1 = b3,    b1 = " << b1 << "\r\n";

    b2 = false;
    buffer << "after b2 = false, b2 = " << b2 << "\r\n\r\n";

    if (b1)
        buffer << "b1 evaluates to true\r\n";
    else
        buffer << "b1 does NOT evaluate to true!\r\n";

    if (b2)
        buffer << "b2 does NOT evaluate to false!\r\n";
    else
        buffer << "b2 evaluates to false\r\n";

    if (b1 == b2)
        buffer << "b1 should NOT equal b2\r\n";
    else
        buffer << "b1 not equal to b2\r\n";

    if (b1 != b2)
        buffer << "b1 not equal to b2\r\n";
    else
        buffer << "b1 should NOT equal b2\r\n";

    if (b1 == b3)
        buffer << "b1 equals b3\r\n";
    else
        buffer << "b1 NOT equal to b3!\r\n";

    if (b1 != b3)
        buffer << "b1 NOT equal to b3!\r\n";
    else
        buffer << "b1 equals b3\r\n";

    buffer << "\r\n"
           << "  int(b1) = " <<   int(b1) << "\r\n"
           << " long(b2) = " <<  long(b2) << "\r\n"
           << "float(b3) = " << float(b3) << "\r\n";
    }

//-----------------------
// Test the RandDev class
//-----------------------

void TestRandDev
    (
    strstream & buffer
    )
    {
    buffer << "Uniform Deviate Test\r\n"
           << "--------------------\r\n\r\n"
           << dec << setprecision(6);

    const int TEST_SIZE  = 31231;
    const int ARRAY_SIZE =  1000;

    float chiSq;
    int i, j;
    unsigned int numbers[ARRAY_SIZE];

    RandDev udev;

    // perform a chi-square test
    chiSq = 0.0F;

    // fill array with zeros
    for (i = 0; i < ARRAY_SIZE; ++i)
         numbers[i] = 0;

    // increment
    for (i = 0; i < TEST_SIZE; ++i)
         numbers[size_t(udev() * ARRAY_SIZE)]++;

    // calculate chi-square value
    for (i = 0; i < ARRAY_SIZE; ++i)
         chiSq += (float)numbers[i] * (float)numbers[i];

    chiSq = (ARRAY_SIZE * chiSq / TEST_SIZE) - TEST_SIZE;

    // display chi-square value
    buffer << "Chi-Square test value: " << chiSq << "\r\n\r\n";

    // output a set of values for each generator
    for (i = 0; i < 20; ++i)
        {
        for (j = 0; j < 5; ++j)
            {
            buffer << '\t' << setw(10) << udev();
            }

        buffer << "\r\n";
        }
    }

//--------------------------------------------
// Test floating point Mutation and Crossover
//--------------------------------------------

void TestMutableFloat
    (
    strstream & buffer
    )
    {
    buffer << "Test floating point Mutation & Crossover\r\n"
              "----------------------------------------\r\n\r\n";

    static const long FSign = 0x80000000L;
    static const long FExpt = 0x7F800000L;
    static const long FMant = 0x007FFFFFL;

    float f[4] = { 0.0F, 1.0F, 2.0F, -0.0F };
    long  l[4] = { 0, 0, 0, 0 };
    size_t i, j;

    RandDev devgen;

    memcpy(l,f,4 * sizeof(float));

    buffer << setprecision(6);

    for (i = 0; i < 4; ++i)
        {
        buffer << setw(4) << f[i] << " = "
               << hex << l[i]
               << " (sign " << (l[i] & FSign)
               << ", expt " << (l[i] & FExpt)
               << ", mant " << (l[i] & FMant)
               << ")\r\n";
        }

    // create hundreds of mutant floating-point numbers
    buffer << dec     << "\r\n"
           << setw(4)  <<  0  << ": "
           << setw(12) << f[0] << " "
           << setw(12) << f[1] << " "
           << setw(12) << f[2] << " "
           << setw(12) << f[3] << "\r\n";

    FloatMutagen fmute;

    for (i = 1; i <= 100; ++i)
        {
        buffer << setw(4) << i << ":";

        for (j = 0; j < 4; ++j)
            {
            f[j] = fmute.Mutate(f[j]);
            buffer << " " << setw(12) << f[j];
            }

        buffer << "\r\n";
        }

    // test crossover
    buffer << "\r\n";

    size_t p1, p2;

    for (i = 0; i < 100; ++i)
        {
        p1 = size_t(devgen() * 4.0F);
        p2 = size_t(devgen() * 4.0F);

        buffer << dec
               << setw(12) << f[p1]  << " cross "
               << setw(12) << f[p2]  << " is "
               << setw(12) << Crossover(f[p1],f[p2]) << "\r\n";
        }
    }

//-----------------------------------
// Test double Mutation and Crossover
//-----------------------------------

void TestMutableDouble
    (
    strstream & buffer
    )
    {
    buffer << "Test double Mutation & Crossover\r\n"
              "--------------------------------\r\n\r\n";

    buffer << setprecision(15);

    static const long DSign = 0x80000000L;
    static const long DExpt = 0x7FF00000L;
    static const long DMant = 0x000FFFFFL;

    double f[4]    = { 0.0, 1.0, 2.0, -0.0 };
    long   l[4][2] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} };
    size_t i, j;

    RandDev devgen;

    memcpy(l,f,4 * sizeof(double));

    for (i = 0; i < 4; ++i)
        {
        buffer << setw(4) << f[i] << " = "
               << hex
               << l[i][0] << " " << l[i][1]
               << " (sign " << (l[i][1] & DSign)
               << ", expt " << (l[i][1] & DExpt)
               << ", mant " << (l[i][1] & DMant)
               << " " << l[i][0] << ")\r\n";
        }

    // create hundreds of mutant floating-point numbers
    buffer << dec      << "\r\n"
           << setw(4)  << 0    << ": "
           << setw(22) << f[0] << " "
           << setw(22) << f[1] << " "
           << setw(22) << f[2] << " "
           << setw(22) << f[3] << "\r\n";

    FloatMutagen fmute;

    for (i = 1; i <= 100; ++i)
        {
        buffer << setw(4) << i << ":";

        for (j = 0; j < 4; ++j)
            {
            f[j] = fmute.Mutate(f[j]);
            buffer << " " << setw(22) << f[j];
            }

        buffer << "\r\n";
        }

    // test crossover
    buffer << "\r\n";

    size_t p1, p2;

    for (i = 0; i < 100; ++i)
        {
        p1 = size_t(devgen() * 4.0F);
        p2 = size_t(devgen() * 4.0F);

        buffer << dec
               << setw(22) << f[p1]  << " cross "
               << setw(22) << f[p2]  << " is "
               << setw(22) << Crossover(f[p1],f[p2]) << "\r\n";
        }
    }

//------------------------------
// Test floating-point functions
//------------------------------

void TestMath
    (
    strstream & buffer
    )
    {
    buffer << "Test floating-point functions\r\n"
              "-----------------------------\r\n\r\n";

    buffer << setprecision(7) << dec;

    unsigned int cw = _control87(0,0);

    buffer << hex << "0x" << cw
           << dec << "\r\n\r\n";

    for (double d = 0.0; d < 13.0; d += (N_DPI / 8.0))
        {
        buffer << setw(12) << d
               << ": sin = " << setw(12) << sin(d)
               <<  " cos = " << setw(12) << cos(d)
               <<  " tan = " << setw(12) << tan(d)
               << "\r\n";
        }

    buffer << setprecision(FLT_DIG) << "\r\n";
    buffer << " float min = " << FLT_MIN << "\r\n";
    buffer << " float max = " << FLT_MAX << "\r\n";
    buffer << " float eps = " << FLT_EPSILON << "\r\n";

    buffer << setprecision(DBL_DIG) << "\r\n";
    buffer << "double min = " << DBL_MIN << "\r\n";
    buffer << "double max = " << DBL_MAX << "\r\n";
    buffer << "double eps = " << DBL_EPSILON << "\r\n";
    }

//-----------------------
// Test sorting functions
//-----------------------

void TestSorting
    (
    strstream & buffer
    )
    {
    buffer << "Test sorting template functions\r\n"
              "-------------------------------\r\n\r\n"
            << setprecision(7) << dec;

    bool r;
    size_t i, n;
    RandDev devgen;
    float a = devgen();
    float b = devgen();
    float c = devgen();
    float array[100];

    for (i = 0; i < 100; ++i)
        array[i] = devgen();

    buffer << "before sorts:"
           << " a = " << a
           << " b = " << b
           << " c = " << c
           << "\r\n";

    // test two-sort
    SortTwo(a,b);

    buffer << "after 2-sort:"
           << " a = " << a
           << " b = " << b
           << "\r\n";

    // test three-sort
    SortThree(a,b,c);

    buffer << "after 3-sort:"
           << " a = " << a
           << " b = " << b
           << " c = " << c
           << "\r\n";

    // test shell sort
    ShellSortDescending(array,100U);

    buffer << "\r\narray after descending shell sort:\r\n";

    for (i = 0; i < 100; ++i)
        buffer << array[i] << "\r\n";

    ShellSortAscending(array,100U);

    buffer << "\r\narray after ascending shell sort:\r\n";

    for (i = 0; i < 100; ++i)
        buffer << array[i] << "\r\n";

    // binary search
    buffer << "\r\n";

    for (i = 0; i < 100; ++i)
        {
        try {
            n = BinSearch(array[i],array,100U);
            buffer << "(" << i << ") n = " << n << "\r\n";
            }
        catch (BinSearchEx)
            {
            buffer << "!!!! Couldn't find array[" << i << "]\r\n";
            }
        }

    try {
        BinSearch(0.5F,array,100U);
        buffer << "!!!! Found nonexistent number!\r\n";
        }
    catch (BinSearchEx)
        {
        buffer << "Didn't find nonexistent number in array\r\n";
        }

    // binary find
    buffer << "\r\n";

    for (i = 0; i < 100; ++i)
        {
        r = BinFind(array[i],array,100U);
        buffer << "(" << i << ") " << r << "\r\n";
        }

    r = BinFind(0.5F,array,100U);

    if (r)
        buffer << "!!!! Found nonexistent number!\r\n";
    else
        buffer << "Didn't find nonexistent number in array\r\n";
    }

//---------------------------
// Test finite state machines
//---------------------------

void TestFSM
    (
    strstream & buffer
    )
    {
    buffer << "Test finite state machines\r\n"
              "--------------------------\r\n\r\n"
            << setprecision(7) << dec;

    const size_t isize = 2;
    const size_t osize = 3;
    char   iset[2] = { '0','1' };
    char   oset[3] = { 'A','B','Y' };
    char   data[6] = { '0','1','1','1','0','1' };

    FSM_TranData<char> tdata[3][isize] =
        {
        {FSM_TranData<char>(1,'B'),FSM_TranData<char>(0,'B')},
        {FSM_TranData<char>(1,'Y'),FSM_TranData<char>(2,'A')},
        {FSM_TranData<char>(1,'B'),FSM_TranData<char>(0,'Y')}
        };

    EVFSM_MuteWts wts = { 10.0F, 10.0F, 10.0F, 10.0F, 10.0F };

    FiniteStateMachine<char,isize,char> fsm(3,&tdata[0][0],iset,0);

    for (size_t i = 0; i < 6; ++i)
        {
        buffer << "i: " << data[i]
               << " o: " << fsm.Transition(data[i])
               << "\r\n";
        }

    buffer << "\r\n";

    try {
        fsm.Transition('X');
        buffer << "!BAD! Erroneous state NOT caught.\r\n";
        }
    catch (FSM_Ex)
        {
        buffer << "Good! Erroneous state caught.\r\n";
        }

    buffer << "\r\n";
    fsm.DumpStructure(buffer);

    // check out mutable FSMs
    EvolvingFSM<char,isize,char,osize> gfsm(3,&tdata[0][0],iset,oset,0,wts);
    EvolvingFSM<char,isize,char,osize> * child;

    buffer << "\r\n--------------------\r\n";
    gfsm.DumpStructure(buffer);

    child = new EvolvingFSM<char,isize,char,osize>(gfsm);

    // mutate
    for (i = 0; i < 12; ++i)
        {
        buffer << "\r\n-------- Mutation: ";

        switch (child->Mutate(1,10))
            {
            case EVFSM_OutSymbol:
                buffer << "Output Symbol\r\n";
                break;
            case EVFSM_Transition:
                buffer << "Transition\r\n";
                break;
            case EVFSM_AddState:
                buffer << "Add State\r\n";
                break;
            case EVFSM_DelState:
                buffer << "Delete State\r\n";
                break;
            case EVFSM_InitState:
                buffer << "Initial State\r\n";
                break;
            case EVFSM_None:
                buffer << "None\r\n";
                break;
            default:
                buffer << "UNKNOWN\r\n";
            }

        child->DumpStructure(buffer);
        }

    delete child;
    }

