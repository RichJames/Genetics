#include <iostream>
#include "gafloat.h"

using namespace std;

int main() {
	double d	= -2.6953229979919913;
	//double d1	= 0.08674326;
	//double d2	= 3.03837279;

	FloatMutagen fmute	= FloatMutagen();
	double dMutant;
	//double dcross;

	cout.precision(17);
	//for (int i=0; i < 20; i++)
	{
		dMutant		= fmute.Mutate(d);
		cout << "dMutant = " << dMutant << endl;
		//dcross		= Crossover(d1, d2);
		//cout << "dcross = " << dcross << endl;
	}

	return 0;
}
