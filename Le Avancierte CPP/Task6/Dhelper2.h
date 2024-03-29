#pragma once
#include "TestLevel.h"
#include <string>
#include <limits.h>

#if DEL == 1
#define assert2(X) 
#elif DEL == 2
#define assert2(X) assert(X)
#endif

namespace {
    //DEL's value control the behavior (is there a default constructor or not)
    //We assume that unconstructed memory has a content <=0.
    // throws otherwise
    // A Dhelper2 object has a char value and a status flag with values:
    enum {
        NON = 1, //Not initialized at all
        DD, // Has been destroyed
        MF, // Has been moved From
        DC, // Default constructed (onlypossible if DEL == 1)
        CV, // Constructed with value
        CC, // Copy Constructed
        MC, // Move Constructed
        CA, // Copy assigned
        MA, // Move assigned
    };
    // NON 0 //Not initialized at all
    //#define DD 1 // Has been destroyed
    //#define MF 2 // Has been moved From
    //#define DC 3 // Default constructed (onlypossible if DEL == 1)
    //#define CV 4 // Constructed with value
    //#define CC 5 // Copy Constructed
    //#define MC 6 // Move Constructed
    //#define CA 7 // Copy assigned
    //#define MA 8 // Move assigned

    struct Dhelper2 {
        static std::string usedConstr;
        static bool checkDhelper;
        char flag;
        char FLAG;
        char value;
        int Test() { return 1; }
        const int Test() const { return 2; }
        void IsConstr(bool checkDhelper) {
            assert(!checkDhelper || 1 < flag && flag < 7);
        }
        void IsNotConstr(bool checkDhelper)
        {
            assert(!checkDhelper || 1 == flag);
        }
        bool IsConstr() const {
#if DEL == 2
            assert(FLAG >= MF);
#endif
            return true;
        }
        bool IsNotConstr() const {
#if DEL == 2
            assert(FLAG <= MF);
#endif
            return true;
        }
        bool IsMoved() const {
#if DEL == 2
            assert(FLAG == MF);
#endif
            return true;
        }

        operator char() const { return value; }

        ~Dhelper2() {
            IsConstr();
            FLAG = DD;
        }
        Dhelper2() {
            assert(FLAG <= DD); // Constructed twice!
            FLAG = CV;
            value = 0;
            usedConstr += "DC";
            IsNotConstr(checkDhelper);
            flag = 2;
        }
        Dhelper2(bool) {
            assert(false);
        }
        Dhelper2(int v) {
            IsNotConstr();
            FLAG = CV;
            value = v;
            usedConstr += "CC";
            IsNotConstr(checkDhelper);
            flag = 3;
        }
        Dhelper2(const Dhelper2& other) {
            assert(FLAG <= DD); // Constructed twice!
            FLAG = CC;
            value = other.value;
            usedConstr += "CC";
            IsNotConstr(checkDhelper);
            flag = 4;
        }
        Dhelper2(Dhelper2&& other) noexcept {
            assert(IsNotConstr());
            assert(other.IsConstr());
            FLAG = MC;
            other.FLAG = MF;
            value = other.value;
            //TODO se om detta �r r�tt
            flag = 6;
            //
        }
        ////Not used
        //Dhelper2& Assign(const Dhelper2& other, bool checkDhelper = true) {
        //	value = other.value;
        //	usedConstr += "CA";
        //	IsConstr(checkDhelper);
        //	flag = 6;
        //	return *this;
        //}
        ////Not used
        //Dhelper2& Assign(char v, bool checkDhelper = true) {
        //	value = v;
        //	usedConstr += "CA";
        //	IsConstr(checkDhelper);
        //	flag = 6;
        //	return *this;
        //}

        Dhelper2& operator=(char v) {
            assert(FLAG > DD); // not constructed!
            FLAG = CA;

            value = v;
            usedConstr += "CA";
            IsConstr(true);
            flag = 6;
            return *this;
        }
        //Used
        Dhelper2& operator=(const Dhelper2& other) {
            assert(FLAG > DD); // not constructed!
            FLAG = CA;
            value = other.value;
            usedConstr += "CA";
            IsConstr(true);
            flag = 6;
            return *this;
        }
        ////Not used
        //Dhelper2& operator=(Dhelper2&& other) noexcept {
        //    assert(FLAG > DD); // not constructed!
        //    FLAG = MA;
        //    value = other.value;
        //    usedConstr += "MA";
        //    IsConstr(true);
        //    flag = 7;
        //    return *this;
        //}
        ////Not used
        //friend bool operator==(const Dhelper2& lhs, const Dhelper2& rhs) {
        //    assert(lhs.FLAG > DD && rhs.FLAG > DD); // Not constructed
        //    return lhs.value == rhs.value;
        //}
        friend bool operator==(const Dhelper2& lhs, char rhs) {
            assert(lhs.FLAG > DD); // Not constructed!
            return lhs.value == rhs;
        }
        friend bool operator!=(const Dhelper2& lhs, const Dhelper2& rhs) {
            assert(lhs.FLAG > DD && rhs.FLAG > DD); // Not constructed!
            auto x = lhs.value != rhs.value;
            return x;
        }

    };

    std::string Dhelper2::usedConstr{};
    bool Dhelper2::checkDhelper{};

}

