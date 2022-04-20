#include "Ident.hpp"

Ident::Ident() {
    declare = false;
    assign  = false;
}

bool Ident::operator== (const string & s) const {
    return name == s;
}

Ident::Ident (const string n) {
    name = n;
    declare = false;
    assign = false;
}

