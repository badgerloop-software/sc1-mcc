#ifndef __MCCSTATELIST_H__
#define __MCCSTATELIST_H__
#include "mbed.h"
#include "mcc.h"
#include "mccstate.h"

class OFF : public MccState {
    public:
        void enter(Mcc* mcc, MccState& newState);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc, MccState& newState);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        OFF() {}
	    OFF(const OFF& other);
	    OFF& operator=(const OFF& other);
};

class PARK : public MccState {
    public:
        void enter(Mcc* mcc, MccState& newState);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc, MccState& newState);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        PARK() {}
	    PARK(const PARK& other);
	    PARK& operator=(const PARK& other);
};

class FORWARD : public MccState {
    public:
        void enter(Mcc* mcc, MccState& newState);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc, MccState& newState);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        FORWARD() {}
	    FORWARD(const FORWARD& other);
	    FORWARD& operator=(const FORWARD& other);
};

class REVERSE : public MccState {
    public:
        void enter(Mcc* mcc, MccState& newState);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc, MccState& newState);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        REVERSE() {}
	    REVERSE(const REVERSE& other);
	    REVERSE& operator=(const REVERSE& other);
};



#endif // __MCCSTATELIST_H__