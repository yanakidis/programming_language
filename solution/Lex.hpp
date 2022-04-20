#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include <unistd.h>

using namespace std;

enum type_of_lex {
    LEX_NULL, LEX_BEGIN, LEX_END, LEX_PROGRAM, LEX_AND, LEX_BOOL, LEX_BREAK, LEX_DO, LEX_IF, LEX_ELSE, LEX_INT, LEX_STRING, LEX_NOT, LEX_OR, 
    LEX_READ, LEX_THEN, LEX_TRUE, LEX_FALSE, LEX_WHILE, LEX_WRITE, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN,
    LEX_EQ, LEX_LSS, LEX_GTR, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_PERCENT, LEX_LBRACKET, LEX_RBRACKET,
    LEX_NUM,
    LEX_ID,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
    int str_nom;
    int stb_nom;
public:
    Lex (type_of_lex t = LEX_NULL, int v = 0, int str = 1, int stb = 1): t_lex (t), v_lex (v), str_nom (str), stb_nom(stb) {}
    type_of_lex get_type () const;
    int get_value () const;
    friend ostream & operator<< ( ostream &s, Lex l );
    friend string & operator<< ( string &s, Lex l );
    int get_str_nom () const;
    int get_stb_nom () const;
};
