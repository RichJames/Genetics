//-----------------------------------------------------------
//  program FOR Genetic algorithm Experimentation (FORGE)
//-----------------------------------------------------------
//
//      forgepd.cpp     v1.00
//
//      Prisoner's Dilemma by bit strings
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
#include "pdoptcfg.h" // configuration dialog box
#include "fstream.h"  // external file stream classes
#include "bintreek.h" // keyed binary tree container

//-------------------
// Prisoner's Dilemma
//-------------------

typedef unsigned long Chromosome;

struct CData
    {
    size_t     Level;
    Chromosome Chrom;

    CData()
        {
        Level = 0;
        Chrom = 0UL;
        }

    CData
        (
        size_t     l,
        Chromosome c
        )
        {
        Level = l;
        Chrom = c;
        }

    bool operator ==
        (
        CData c
        ) const
        {
        if ((Chrom == c.Chrom) && (Level == c.Level))
            return true;
        else
            return false;
        }

    bool operator !=
        (
        CData c
        ) const
        {
        if ((Chrom != c.Chrom) || (Level != c.Level))
            return true;
        else
            return false;
        }

    bool operator <
        (
        CData c
        ) const
        {
        if (Level < c.Level)
            return true;

        if ((Level == c.Level) && (Chrom < c.Chrom))
            return true;

        return false;
        }
    };

void TestDilemma
    (
    HINSTANCE inst,
    HWND      wdw,
    strstream & buffer
    )
    {
    buffer << "Prisoner's Dilemma\r\n"
              "------------------\r\n";

    // create configuration and verify it
    PDOptConfig pdoc(inst,wdw);

    if (!pdoc.GetValidity())
        {
        buffer << "Cancelled\r\n";
        return;
        }

    // open output file (if required)
    ofstream * fout = NULL;

    if  (pdoc.GetFileOut())
        {
        fout = new ofstream (pdoc.GetFileName());

        if (fout == NULL)
            {
            buffer << "Memory allocation failed\r\n";
            return;
            }
        }

// display parameters
    size_t POP_SZ  = pdoc.GetPopSize();
    size_t GEN_SZ  = pdoc.GetTestSize();
    size_t s;

    switch (pdoc.GetStartLvl())
        {
        case  1: s =     8U; break;
        case  2: s =   128U; break;
        case  3: s = 32768U; break;
        default: s =     2U;
        }

    if (pdoc.GetFairStart() && (POP_SZ % s))
        POP_SZ = (POP_SZ / s + 1) * s;

    buffer << "\r\n Population: " << POP_SZ;
    buffer << "\r\n  Test Size: " << GEN_SZ;
    buffer << "\r\nReport Freq: " << pdoc.GetReptFreq();
    buffer << "\r\nStart Level: " << pdoc.GetStartLvl();

    buffer << "\r\n  Crossover: " << pdoc.GetCrossover();
    if (pdoc.GetCrossover())
        buffer << " (" << pdoc.GetCrossRate() * 100.0F << "%)";

    buffer << "\r\n   Mutation: "<< pdoc.GetMutation();
    if (pdoc.GetMutation())
        buffer << " (" << pdoc.GetMuteRate() * 100.0F << "%)";

    buffer << "\r\n   Doubling: " << pdoc.GetDoubling();
    if (pdoc.GetDoubling())
        {
        buffer << " (" << pdoc.GetDblRate() * 100.0F << "%)";
        buffer << "\r\n Max. Level: " << pdoc.GetMaxLevel();
        }

    buffer << "\r\n  Averaging: " << pdoc.GetAveraging();
    buffer << "\r\n Fair Start: " << pdoc.GetFairStart();
    buffer << "\r\n Random 1st: " << pdoc.GetRandom1st();
    buffer << "\r\n  Payoff DD: " << pdoc.GetPayoffDD();
    buffer << "\r\n  Payoff DC: " << pdoc.GetPayoffDC();
    buffer << "\r\n  Payoff CD: " << pdoc.GetPayoffCD();
    buffer << "\r\n  Payoff CC: " << pdoc.GetPayoffCC();
    buffer << "\r\nFile Output: " << pdoc.GetFileOut();

    if (pdoc.GetFileOut())
        buffer << " (" << pdoc.GetFileName() << ")";

    buffer << "\r\n\r\n" << dec;

    if (fout != NULL)
        {
        (*fout) << "\n Population: " << pdoc.GetPopSize();
        (*fout) << "\n  Test Size: " << pdoc.GetTestSize();
        (*fout) << "\nReport Freq: " << pdoc.GetReptFreq();
        (*fout) << "\nStart Level: " << pdoc.GetStartLvl();

        (*fout) << "\n  Crossover: " << pdoc.GetCrossover();
        if (pdoc.GetCrossover())
            (*fout) << " (" << pdoc.GetCrossRate() * 100.0F << "%)";

        (*fout) << "\n   Mutation: "<< pdoc.GetMutation();
        if (pdoc.GetMutation())
            (*fout) << " (" << pdoc.GetMuteRate() * 100.0F << "%)";

        (*fout) << "\n   Doubling: " << pdoc.GetDoubling();
        if (pdoc.GetDoubling())
            {
            (*fout) << " (" << pdoc.GetDblRate() * 100.0F << "%)";
            (*fout) << "\n Max. Level: " << pdoc.GetMaxLevel();
            }

        (*fout) << "\n  Averaging: " << pdoc.GetAveraging();
        (*fout) << "\n Fair Start: " << pdoc.GetFairStart();
        (*fout) << "\n Random 1st: " << pdoc.GetRandom1st();
        (*fout) << "\n  Payoff DD: " << pdoc.GetPayoffDD();
        (*fout) << "\n  Payoff DC: " << pdoc.GetPayoffDC();
        (*fout) << "\n  Payoff CD: " << pdoc.GetPayoffCD();
        (*fout) << "\n  Payoff CC: " << pdoc.GetPayoffCC();
        (*fout) << "\nFile Output: " << pdoc.GetFileOut();

        if (pdoc.GetFileOut())
            (*fout) << " (" << pdoc.GetFileName() << ")";

        (*fout) << dec << "\n\n";
        }

    // create random deviate and mutation objects
    RandDev devgen;

    // allocate population and fitness arrays
    Chromosome * pop = new Chromosome [POP_SZ];

    if (pop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    Chromosome * newpop = new Chromosome [POP_SZ];

    if (newpop == NULL)
        {
        buffer << "Memory allocation failed\r\n";
        return;
        }

    size_t * lvl = new size_t [POP_SZ];

    if (lvl == NULL)
            {
            buffer << "Memory allocation failed\r\n";
            return;
            }

    size_t * newlvl = new size_t [POP_SZ];

    if (newlvl == NULL)
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

    // shifts for move selection
    static const int shift[4] = { 1, 3, 7, 15 };

    // level masks
    static const Chromosome lmask[5] =
        {
        0x00000001UL,
        0x00000007UL,
        0x0000007FUL,
        0x00007FFFUL,
        0x7FFFFFFFUL
        };

    static const float  lbits[5]  = { 1.0F, 3.0F, 7.0F, 15.0F, 31.0F };
    static const size_t lbitsn[5] = { 1, 3, 7, 15, 31 };

    // strategy masks
    static const Chromosome smask[5] =
        {
        0x00000001UL,
        0x00000006UL,
        0x00000078UL,
        0x00007F80UL,
        0x7FFF8000UL
        };

    static const int sbits[5] = { 1, 2, 4, 8, 16 };

    // various variables
    size_t g, i, j, k, l, p1, p2, pl, ps;
    Chromosome strati, stratj, tempi, tempj, bit, m, vb;
    char buf[64];
    RouletteWheel<double> * rw;

    // mask off low bit if start is random
    if (pdoc.GetRandom1st())
        vb = 0xFFFFFFFEUL;
    else
        vb = 0xFFFFFFFFUL;

    // generate initial 3-bit population
    k = 0;

    if (pdoc.GetFairStart())
        {
        for (i = 0; i < POP_SZ / s; ++i)
            {
            for (m = 0; m < s; ++m)
                {
                pop[k] = m;
                lvl[k] = pdoc.GetStartLvl();
                ++k;
                }
            }
        }
    else
        {
        for (i = 0; i < POP_SZ; ++i)
            {
            pop[i] = Chromosome(devgen() * float(s));
            lvl[i] = pdoc.GetStartLvl();
            }
        }

    // do the generations
    for (g = 0; g < GEN_SZ; ++g)
        {
        // display progress in app header
        wsprintf(buf,"Forge (loop: %u of %u)",g,GEN_SZ);
        SetWindowText(wdw,buf);

        // calculate fitness for x values
        for (i = 0; i < POP_SZ; ++i)
            fit[i] = 0.0;

        for (i = 0; i < POP_SZ; ++i)
            {
            for (j = i + 1; j < POP_SZ; ++j)
                {
                // compete
                l = (lvl[i] < lvl[j]) ? lvl[i] : lvl[j];

                // level 1
                if (pdoc.GetRandom1st())
                    {
                    strati = (devgen() > 0.5F) ? 1 : 0;
                    stratj = (devgen() > 0.5F) ? 1 : 0;
                    }
                else
                    {
                    strati = pop[i] & 1UL;
                    stratj = pop[j] & 1UL;
                    }

                // levels 2 through 5
                for (k = 0; k < l; ++k)
                    {
                    // select my move
                    bit   = pop[i] & (1UL << int(shift[k] + stratj));
                    tempi = (strati << 1) | (bit >> int(shift[k] + stratj));

                    // select his move
                    bit   = pop[j] & (1UL << int(shift[k] + strati));
                    tempj = (stratj << 1) | (bit >> int(shift[k] + strati));

                    strati = tempi;
                    stratj = tempj;
                    }

                // compete
                if (pdoc.GetRandom1st())
                    {
                    k = 1;
                    m = 2;
                    }
                else
                    {
                    k = 0;
                    m = 1;
                    }

                for (; k <= l; ++k)
                    {
                    if (strati & m)
                        {
                        if (stratj & m)
                            {
                            fit[i] += pdoc.GetPayoffCC();
                            fit[j] += pdoc.GetPayoffCC();
                            }
                        else
                            {
                            fit[i] += pdoc.GetPayoffCD();
                            fit[j] += pdoc.GetPayoffDC();
                            }
                        }
                    else
                        {
                        if (stratj & m)
                            {
                            fit[i] += pdoc.GetPayoffDC();
                            fit[j] += pdoc.GetPayoffCD();
                            }
                        else
                            {
                            fit[i] += pdoc.GetPayoffDD();
                            fit[j] += pdoc.GetPayoffDD();
                            }
                        }

                    m <<= 1;
                    }
                }

            // scale fitness to number of bits tested
            if (pdoc.GetAveraging())
                fit[i] /= float(l+(pdoc.GetRandom1st() ? 0 : 1));
            }

        // display results
        if ((fout != NULL) || ((g % pdoc.GetReptFreq()) == 0))
            {
            if ((g % pdoc.GetReptFreq()) == 0)
                buffer << "\r\nGeneration " << g << "\r\n";

            if (fout != NULL)
                (*fout) << "\nGeneration " << g << "\n";

            BinaryTreeKeyed < CData, size_t > tree;

            for (i = 0; i < POP_SZ; ++i)
                {
                CData d(lvl[i],(pop[i] & vb));

                try {
                    j = tree.LookUp(d);
                    ++j;
                    tree.Insert(d,j);
                    }
                catch (TreeEx & ex)
                    {
                    if (ex.WhatsWrong() == BTX_NOTFOUND)
                        tree.Insert(d,1);
                    else
                        throw;
                    }
                }

            BinaryTreeKeyedIterator < CData, size_t > iter(tree);

            while (1)
                {
                try {
                    CData d(iter.GetKey());

                    if ((g % pdoc.GetReptFreq()) == 0)
                        buffer << setw(5) << (*iter) << ": ";

                    if (fout != NULL)
                        (*fout) << setw(5) << (*iter) << ": ";

                    m = 1UL;

                    for (j = 0; j < lbitsn[d.Level]; ++j)
                        {
                        if ((j == 0) && pdoc.GetRandom1st())
                            {
                            m <<= 1;
                            continue;
                            }

                        if ((g % pdoc.GetReptFreq()) == 0)
                            buffer << ((d.Chrom & m) ? 'C' : 'D');

                        if (fout != NULL)
                            (*fout) << ((d.Chrom & m) ? 'C' : 'D');

                        if ((j == 0) || (j == 2)
                        ||  (j == 6) || (j == 14))
                            {
                            if ((g % pdoc.GetReptFreq()) == 0)
                                buffer << ' ';

                            if (fout != NULL)
                                (*fout) << ' ';
                            }

                        m <<= 1;
                        }

                    if ((g % pdoc.GetReptFreq()) == 0)
                        buffer << "\r\n";

                    if (fout != NULL)
                        (*fout) << "\n";

                    ++iter;
                    }
                catch (TreeEx & ex)
                    {
                    if (ex.WhatsWrong() == BTX_NOTFOUND)
                        break;
                    else
                        throw;
                    }
                }
            }

        // create new generation
        rw = new RouletteWheel<double> (POP_SZ,fit);

        if (rw == NULL)
            {
            buffer << "Failed to allocate roulette wheel\r\n";
            return;
            }

        for (i = 0; i < POP_SZ; ++i)
            {
            // select a parent
            p1 = rw->GetIndex();

            // crossover
            if (pdoc.GetCrossover()
            &&  (devgen() < pdoc.GetCrossRate()))
                {
                // get second parent
                p2 = rw->GetIndex();

                // find longer of two strings
                if (lvl[p1] >= lvl[p2])
                    {
                    l  = lvl[p2];
                    pl = p1;
                    ps = p2;
                    }
                else
                    {
                    l  = lvl[p1];
                    pl = p2;
                    ps = p1;
                    }

                // create crossover bitmask
                m = lmask[l] >> int(devgen() * lbits[l]);

                // combine for new child
                newpop[i] = (pl & (~m)) | (ps & m);
                newlvl[i] = lvl[pl];
                }
            else
                {
                newpop[i] = pop[p1];
                newlvl[i] = lvl[p1];
                }

            // doubling
            if (pdoc.GetDoubling()
            &&  (newlvl[i] < pdoc.GetMaxLevel())
            &&  (devgen()  < pdoc.GetDblRate()))
                {
                bit        = newpop[i] & smask[newlvl[i]];
                newpop[i] |= (bit <<  sbits[newlvl[i]]);
                newpop[i] |= (bit << (sbits[newlvl[i]] * 2));
                ++newlvl[i];
                }

            // mutation
            if (pdoc.GetMutation()
            &&  (devgen() < pdoc.GetMuteRate()))
                {
                m = 1UL << int(devgen() * lbits[newlvl[i]]);

                if (newpop[i] & m)
                    newpop[i] &= (~m);
                else
                    newpop[i] |= m;
                }

            newpop[i] &= lmask[newlvl[i]];
            }

        delete rw;

        // copy new generation
        memcpy(pop,newpop,POP_SZ * sizeof(Chromosome));
        memcpy(lvl,newlvl,POP_SZ * sizeof(size_t));
        }

    // remove arrays
    delete [] fit;
    delete [] newlvl;
    delete [] lvl;
    delete [] newpop;
    delete [] pop;
    delete    fout;

    // restore window text
    SetWindowText(wdw,"Forge");
    }
