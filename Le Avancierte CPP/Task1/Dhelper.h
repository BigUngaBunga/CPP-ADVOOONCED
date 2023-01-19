#pragma once
#include <string>
#include <cassert>

namespace {
    //We assume that Dalloc set unused memory to 1
    //Varning, denna typ funkar inte som classer brukar göra!
    //Dhelper d; kommer att ge runtime error!
    struct Dhelper {
        static std::string usedConstr;
        static bool check;
        char flag;
        char value;
        int Test() { return 1; }
        const int Test() const { return 2; }
        void IsConstr(bool check) {
            assert(!check || 1 < flag && flag < 7);
        }
        void IsNotConstr(bool check) 
        {
            assert(!check || !(1 < flag && flag < 7));
        }

        operator char() const { return value; }

        ~Dhelper() {
            IsConstr(true);
            flag = 1;
        }
        Dhelper(bool check = true) {
            value = 0;
            usedConstr += "DC";
            IsNotConstr(check);
            flag = 2;
        }
        Dhelper(int v, bool check = true) {
            value = v;
            usedConstr += "CC";
            IsNotConstr(check);
            flag = 3;
        }
        Dhelper(const Dhelper& other, bool check = true) {
            value = other.value;
            usedConstr += "CC";
            IsNotConstr(check);
            flag = 4;
        }
        Dhelper(Dhelper&& other, bool check = true) noexcept {
            value = other.value;
            usedConstr += "MC";
            IsNotConstr(check);
            flag = 5; 
        }
        //Not used
        Dhelper& Assign(const Dhelper& other, bool check = true) {
            value = other.value;
            usedConstr += "CA";
            IsConstr(check);
            flag = 6;
            return *this;
        }
        //Not used
        Dhelper& Assign(char v, bool check = true) {
            value = v;
            usedConstr += "CA";
            IsConstr(check);
            flag = 6;
            return *this;
        }

        Dhelper& operator=(char v) {
            value = v;
            usedConstr += "CA";
            IsConstr(true);
            flag = 6;
            return *this;
        }
        //Used
        Dhelper& operator=(const Dhelper& other) {
            value = other.value;
            usedConstr += "CA";
            IsConstr(true);
            flag = 6;
            return *this;
        }
        //Not used
        Dhelper& operator=(Dhelper&& other) noexcept {
            value = other.value;
            usedConstr += "MA";
            IsConstr(true);
            flag = 7;
            return *this;
        }
        //Not used
        friend bool operator==(const Dhelper& lhs, const Dhelper& rhs) {
            return lhs.value == rhs.value;
        }
        friend bool operator==(const Dhelper& lhs, char rhs) {
            return lhs.value == rhs;
        }
        friend bool operator!=(const Dhelper& lhs, const Dhelper& rhs) {
            auto x= lhs.value != rhs.value;
            return x;
        }

    };

    std::string Dhelper::usedConstr{};
    bool Dhelper::check{};

}

