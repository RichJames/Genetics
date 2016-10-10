#include <iostream>
#include <string>

using namespace std;

//#include "windows.h"  // Windows definitions
//#include "forgeres.h" // resource constants
//#include "strstrea.h" // strstream definitions
//#include "iomanip.h"  // stream manipulators
//#include "string.h"   // memory management functions
//#include "randdev.h"  // uniform random deviate generator
//#include "roulette.h" // RouletteWheel class
//#include "bool.h"     // ANSI-like 'bool' class
//#include "pdoptcfg.h" // configuration dialog box
//#include "fstream.h"  // external file stream classes
//#include "bintreek.h" // keyed binary tree container

//-------------------
// Prisoner's Dilemma
//-------------------

typedef unsigned long Chromosome;

void test()
{
    size_t POP_SZ  = 2;
    size_t GEN_SZ  = 2;
    size_t s;

    switch (3)
        {
        case  1: s =     8U; break;
        case  2: s =   128U; break;
        case  3: s = 32768U; break;
        default: s =     2U;
        }

    // allocate population and fitness arrays
    Chromosome * pop = new Chromosome [POP_SZ];

    if (pop == NULL)
        {
        cout << "Memory allocation failed\r\n";
        return;
        }

    Chromosome * newpop = new Chromosome [POP_SZ];

    if (newpop == NULL)
        {
        cout << "Memory allocation failed\r\n";
        return;
        }

    size_t * lvl = new size_t [POP_SZ];

    if (lvl == NULL)
            {
            cout << "Memory allocation failed\r\n";
            return;
            }

    size_t * newlvl = new size_t [POP_SZ];

    if (newlvl == NULL)
        {
        cout << "Memory allocation failed\r\n";
        return;
        }

    double * fit = new double [POP_SZ];

    if (fit == NULL)
        {
        cout << "Memory allocation failed\r\n";
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
    vb = 0xFFFFFFFFUL;

    // generate initial 3-bit population
    k = 0;
    pop[0] = 0x69cd;  lvl[0] = 3;
    pop[1] = 0x7476;  lvl[1] = 3;
        // calculate fitness for x values
        for (i = 0; i < POP_SZ; ++i)
            fit[i] = 0.0;

        for (i = 0; i < POP_SZ; ++i)
            {
		cout << "for outer loop:  i = " << i << endl;
            for (j = i + 1; j < POP_SZ; ++j)
                {
			string pad = "  ";
			cout << pad << "for inner loop:  j = " << j << endl;
	
                // compete
                l = (lvl[i] < lvl[j]) ? lvl[i] : lvl[j];
			cout << pad << "l = " << l << endl;

                // level 1
                    strati = pop[i] & 1UL;
                    stratj = pop[j] & 1UL;
			cout << pad << "pop[" << i << "] = " << pop[i] << endl;
			cout << pad << "pop[" << j << "] = " << pop[j] << endl;
			cout << pad << "level 1 strati = " << strati << endl;
			cout << pad << "level 1 stratj = " << stratj << endl;

                // levels 2 through 5
                for (k = 0; k < l; ++k)
                    {
			string pad2 = pad + pad;
			cout << pad2 << "k loop: k = " << k << endl;
			cout << pad2 << "shift[" << k << "] = " << shift[k] << endl;
		
                    // select my move
                    bit   = pop[i] & (1UL << int(shift[k] + stratj));
                    tempi = (strati << 1) | (bit >> int(shift[k] + stratj));

			cout << pad2 << "=== my move ===" << endl;
                    	cout << pad2 << "bit = pop[i] & (1UL << int(shift[k] + stratj))" << endl;
			cout << pad2 << bit << " = " 
				     << pop[i] << " & (1UL << int(" << shift[k] << " + " << stratj << "))" << endl;
                    	cout << pad2 << "tempi = (strati << 1) | (bit >> int(shift[k] + stratj))" << endl;
                    	cout << pad2 << tempi << " = (" << strati << " << 1) | (" << bit << " >> "
                    	             << "int(" << shift[k] << " + " << stratj << "))"  << endl;

                    // select his move
                    bit   = pop[j] & (1UL << int(shift[k] + strati));
                    tempj = (stratj << 1) | (bit >> int(shift[k] + strati));

			cout << pad2 << "=== his move ===" << endl;
                    	cout << pad2 << "bit = pop[j] & (1UL << int(shift[k] + strati))" << endl;
			cout << pad2 << bit << " = "
				     << pop[j] << " & (1UL << int(" << shift[k] << " + " << strati << "))" << endl;
                    	cout << pad2 << "tempj = (stratj << 1) | (bit >> int(shift[k] + strati)" << endl;
                    	cout << pad2 << tempj << " = (" << stratj << " << 1) | (" << bit << " >> "
                    	             << "int(" << shift[k] << " + " << strati << "))"  << endl;

                    strati = tempi;
                    stratj = tempj;
			cout << pad2 << "strati = " << strati << endl;
			cout << pad2 << "stratj = " << stratj << endl << endl;
                    }
                }
	}

    // remove arrays
    delete [] fit;
    delete [] newlvl;
    delete [] lvl;
    delete [] newpop;
    delete [] pop;

    }

int main()
{
	test();

	return 0;
}
