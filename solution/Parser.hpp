#pragma once
#include "Scanner.hpp"

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    Scanner      scan;
    stack < type_of_lex >   st_lex;
    string exc_str;
    int c_str_nom;
    int c_stb_nom;
    void  P();
    void  D1();
    void  D();
    void  B();
    void  S();
    void  E1();
    void  E2();
    void  E3();
    void  E4();
    void  E5();
    void  E6();
    void  E7();
    void  dec ( type_of_lex type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_bool ();
    void  check_id_in_read ();
    void  gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();
        c_str_nom = curr_lex.get_str_nom();
        c_stb_nom = curr_lex.get_stb_nom();
    }
public:
    vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};




