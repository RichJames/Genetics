//-----------------------------------------------------------
//  FORGE
//-----------------------------------------------------------
//
//      geneprog.h     v1.00
//
//      Classes for genetic programming
//
//-----------------------------------------------------------
//  Copyright 1995 by Scott Robert Ladd. All rights reserved.
//-----------------------------------------------------------

#ifndef FORGE_GENEPROG_H
#define FORGE_GENEPROG_H

// forward declarations
class GP_operator;

// template for arguments sets
template <size_t N>
    class GP_arglist
        {
        public:
            // constructors
            GP_arglist();

            // copy constructor
            GP_arglist
                (
                const GP_arglist<N> & slst
                );

            // destructor
            ~GP_arglist();

            // assignment operator (deep copy)
            void operator =
                (
                const GP_arglist<N> & slst
                );

            // get one of the arguments
            GP_operator operator []
                (
                size_t argn
                );

            // set one of the arguments
            GP_operator SetOperator
                (
                size_t argn,
                const GP_operator & oper;
                );

            // interrogate list
            size_t GetN() { return N; }

        protected:
            GP_operator A[N];     // elements in list
        };

class GP_operator
    {
    public:
        virtual double Evaluate() = 0;
    };

class GP_number : public GP_operator
    {
    public:
        GP_number (double n) { V = n; }

        virtual double Evaluate();

    protected:
        double V;
    };

class GP_add
    {
    public:

    };
