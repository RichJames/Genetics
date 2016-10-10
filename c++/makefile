testfloat : main.o gafloat.o randdev.o
	g++ -o testfloat main.o gafloat.o randdev.o
gafloat.o : gafloat.cpp gafloat.h randdev.h fputil.h
	g++ -c gafloat.cpp
randdev.o : randdev.cpp randdev.h
	g++ -c randdev.cpp
main.o : main.cpp
	g++ -c main.cpp
fitness.o : fitness.cpp
	g++ -c fitness.cpp
fitness : fitness.o
	g++ -o fitness fitness.o
