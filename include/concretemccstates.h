#ifndef __CONCRETEMCCSTATES_H__
#define __CONCRETEMCCSTATES_H__
#include "mbed.h"
#include "mccstatemachine.h"
#include "mccstateinterface.h"

class MccOff : public MccState {
    public:
        void enter(Mcc* mcc);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        MccOff() {}
	    MccOff(const MccOff& other);
	    MccOff& operator=(const MccOff& other);
};

class MccPark : public MccState {
    public:
        void enter(Mcc* mcc);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        MccPark() {}
	    MccPark(const MccPark& other);
	    MccPark& operator=(const MccPark& other);
};

class MccForward : public MccState {
    public:
        void enter(Mcc* mcc);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        MccForward() {}
	    MccForward(const MccForward& other);
	    MccForward& operator=(const MccForward& other);
};

class MccReverse : public MccState {
    public:
        void enter(Mcc* mcc);
        void toggle(Mcc* mcc);
        void exit(Mcc* mcc);
        static MccState& getInstance(); // returns a pointer to the singleton

    // declare default constructor, copy constructor, and copy assignment operator as private
    private:
        MccReverse() {}
	    MccReverse(const MccReverse& other);
	    MccReverse& operator=(const MccReverse& other);
};



#endif // __CONCRETEMCCSTATES_H__