#include "Lex.hpp"

type_of_lex Lex::get_type() const {
    return t_lex;
}

int Lex::get_value () const {
    return v_lex;
}

int Lex::get_str_nom () const {
    return str_nom;
}

int Lex::get_stb_nom () const{
    return stb_nom;
}

