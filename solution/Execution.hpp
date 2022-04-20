#pragma once
#include "Parser.hpp"

enum poliz_type {ident, integer, str, boolean, other};

class Executer {
    public:
        void execute (vector <Lex> & poliz);
};

struct object {
    poliz_type obj_type;
    int obj_int;
    string obj_str;
    object (poliz_type tmp, int a) {
        obj_int = a;
        obj_type = tmp;
    }
    object (poliz_type tmp, string a) {
        obj_str = a;
        obj_type = tmp;
    }
};
