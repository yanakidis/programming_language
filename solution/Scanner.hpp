#pragma once
#include "Additional.hpp"

class Scanner {
    FILE * fp;
    char c;
    int look (const string buf, const char ** list);
    void gc ();
  public:
    int temp_str_nom;
    int temp_stb_nom;
    static const char * TW [], * TD [];
    Scanner ( const char * program );
    Lex get_lex ();
};

