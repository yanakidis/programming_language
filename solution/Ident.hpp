#pragma once
#include "Lex.hpp"

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident();
    bool operator== (const string& s) const;
    Ident ( const string n );
    string get_name () const {return name;}
    bool get_declare () const {return declare;}
    void put_declare () {declare   = true;}
    type_of_lex get_type () const {return type;}
    void put_type ( type_of_lex t ) {type = t;}
    bool get_assign () const {return assign;}
    void put_assign () {assign = true;}
    int  get_value () const {return value;}
    void put_value (int v) {value = v;}
};
