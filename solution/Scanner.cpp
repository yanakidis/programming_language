#include "Additional.hpp"
#include "Scanner.hpp"

int Scanner::look (const string buf, const char ** list) {
    int i = 0;
    while (list[i]) {
        if (buf == list[i])
            return i;
        ++i;
        }
    return 0;
}

void Scanner::gc() {
    c = fgetc (fp);
}

Scanner::Scanner (const char * program) {
    if ( !(fp = fopen ( program, "r" )) )
        throw  "can't open file" ;
    temp_str_nom = 1;
    temp_stb_nom = 0;
}

const char * Scanner::TW[] = { "", "begin", "end", "program", "and", "boolean", "break", "do", "if", "else", "int", "string", "not", "or",
                      "read", "then", "true", "false", "while", "write", NULL };
const char * Scanner::TD[] = { "", ";", ",", ":", "=", "(", ")", "==", "<", ">", "<=", "!=", ">=", "+", "-", "*", "/", "%",
                      "{", "}", NULL };

ostream & operator << (ostream &s, Lex l) {
    string t;
    if ( l.t_lex <= LEX_WRITE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_SEMICOLON && l.t_lex <= LEX_RBRACKET )
        t = Scanner::TD[ l.t_lex - LEX_SEMICOLON + 1];
    else if ( l.t_lex == LEX_NUM )
        t = "NUMB";
    else if ( l.t_lex == LEX_ID )
        t = TID[l.v_lex].get_name ();
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO )
        t = "!F";
    else if (l.t_lex == LEX_BREAK)
        t = "break";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << " " << l.t_lex << ") line: " << l.str_nom << " column: " << l.stb_nom << endl;
    return s;
}

Lex Scanner::get_lex() {
    enum state {H, IDENT, NUMB, COM,  VKCOM, ALE, NEQ, STR, VEQ, VCOM};
    int d, j;
    string buf;
    string exc_str = "Unexpected symbol - (";
    state CS = H;
    do {
        gc ();
        temp_stb_nom++;
        if (c == '\n') temp_str_nom++;
        switch (CS) {
            case H:
                if (c == EOF) {
                    buf = "end";
                    j = look ( buf, TW);
                    return Lex ( (type_of_lex) j, j, temp_str_nom, temp_stb_nom );
                }
                if (c == '\n') temp_stb_nom = 0;
                else if ( c == ' ' || c== '\r' || c == '\t' );
                else if ( isalpha (c) ) {
                    buf.push_back (c);
                    CS  = IDENT;
                }
                else if ( isdigit (c) ) {
                    d   = c - '0';
                    CS  = NUMB;
                }
                else if (c == '=') {
                    CS = VEQ; //vosmoshno EQ
                }
                else if ( c == '/' ) {
                    CS  = VCOM; //vosmoshno com
                }
                else if ( c == ':' || c == '<' || c == '>' ) {
                    buf.push_back (c);
                    CS  = ALE;
                }
                else if (c == '"') {
                    CS = STR;
                }
                else if (c == '!') {
                    buf.push_back (c);
                    CS  = NEQ;
                }
                else {
                    buf.push_back (c);
                    if ( ( j = look ( buf, TD) ) ) {
                        return Lex ( (type_of_lex)( j + (int) (LEX_SEMICOLON  - 1) ), j, temp_str_nom, temp_stb_nom );
                    } else {
                        exc_str.push_back(c);
                        exc_str += "). ";
                        exc_str += to_string (temp_str_nom);
                        exc_str += ", ";
                        exc_str += to_string (temp_stb_nom);
                        exc_str += "\n";
                        throw exc_str;
                    }
                }
                break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c);
                } else {
                    ungetc ( c, fp );
                    if (c == '\n') {
                        temp_str_nom--;
                        temp_stb_nom=0;
                    }
                    temp_stb_nom--;
                    if ( (j = look ( buf, TW) ) ) {
                        return Lex ( (type_of_lex) j, j, temp_str_nom, temp_stb_nom);
                    } else {
                        j   = put ( buf );
                        return Lex ( LEX_ID, j, temp_str_nom, temp_stb_nom);
                    }
                }
                break;
            case NUMB:
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' );
                } else {
                    ungetc ( c, fp );
                    if (c == '\n') {
                        temp_str_nom--;
                        temp_stb_nom=0;
                    }
                    temp_stb_nom--;
                    return Lex ( LEX_NUM, d, temp_str_nom, temp_stb_nom);
                }
                break;
            case VCOM:
                if (c == '*') CS = COM;
                else {
                    CS = H;
                    ungetc ( c, fp );
                    if (c == '\n') {
                        temp_str_nom--;
                        temp_stb_nom=0;
                    }
                    temp_stb_nom--;
                    if ( ( j = look ( buf, TD) ) ) {
                        return Lex ( (type_of_lex)( j + (int) (LEX_SEMICOLON  - 1) ), j, temp_str_nom, temp_stb_nom);
                    }
                }
                break;
            case COM:
                if ( c == '*') {
                    CS  =  VKCOM;
                }
                else if ( c == EOF) {
                    exc_str.push_back(c);
                    exc_str += "). Undesirable ending of the comment. ";
                    exc_str += to_string (temp_str_nom);
                    exc_str += ", ";
                    exc_str += to_string (temp_stb_nom);
                    exc_str += "\n";
                    throw exc_str;
                }
                break;
            case  VKCOM:
                if ( c == '/' ) {
                    CS  = H;
                } else if ( c == EOF) {
                    exc_str.push_back(c);
                    exc_str += "). Undesirable ending of the comment. ";
                    exc_str += to_string (temp_str_nom);
                    exc_str += ", ";
                    exc_str += to_string (temp_stb_nom);
                    exc_str += "\n";
                    throw exc_str;
                } else CS = COM;
                break;
            case ALE:
                if ( c == '=' ) {
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) (LEX_SEMICOLON - 1) ), j, temp_str_nom, temp_stb_nom );
                } else {
                    ungetc ( c, fp );
                    if (c == '\n') {
                        temp_str_nom--;
                        temp_stb_nom=0;
                    }
                    temp_stb_nom--;
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) (LEX_SEMICOLON  - 1) ), j, temp_str_nom,temp_stb_nom );
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    return Lex ( LEX_NEQ, j, temp_str_nom, temp_stb_nom );
                } else {
                    exc_str.push_back(c);
                    exc_str += "}. Unknown lexeme. ";
                    exc_str += to_string (temp_str_nom);
                    exc_str += ", ";
                    exc_str += to_string (temp_stb_nom);
                    exc_str += "\n";
                    throw exc_str;
                }
                break;
            case VEQ:
                if (c == '=') {
                    buf = "==";
                    if ( ( j = look ( buf, TD) ) ) {
                        return Lex ( (type_of_lex)( j + (int) (LEX_SEMICOLON  - 1) ), j, temp_str_nom, temp_stb_nom );
                    }
                }
                ungetc(c, fp );
                if (c == '\n') {
                    temp_str_nom--;
                    temp_stb_nom=0;
                }
                temp_stb_nom--;
                buf = "=";
                if ( ( j = look ( buf, TD) ) ) {
                    return Lex ( (type_of_lex)( j + (int) (LEX_SEMICOLON  - 1) ), j, temp_str_nom, temp_stb_nom );
                }
                break;
            case STR:
                if (c != '"') {
                    buf.push_back (c);
                    break;
                } else if (c == EOF) {
                    exc_str.push_back(c);
                    exc_str += "). Undesirable ending of the string. ";
                    exc_str += to_string (temp_str_nom);
                    exc_str += ", ";
                    exc_str += to_string (temp_stb_nom);
                    exc_str += "\n";
                    throw exc_str;
                }
                STR_TABLE.push_back(buf);
                return Lex (LEX_STRING, STR_TABLE.size() - 1, temp_str_nom, temp_stb_nom);
                break;
            }
        } while (true);
    }

string & operator << (string &s, Lex l) {
    string t;
    if ( l.t_lex <= LEX_WRITE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_SEMICOLON && l.t_lex <= LEX_RBRACKET )
        t = Scanner::TD[ l.t_lex - LEX_SEMICOLON + 1];
    else if ( l.t_lex == LEX_NUM )
        t = "NUMB";
    else if ( l.t_lex == LEX_ID )
        t = TID[l.v_lex].get_name ();
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO )
        t = "!F";
    else if (l.t_lex == LEX_BREAK)
        t = "break";
    else
        throw l;
    s = (s + "(" + t + "," + to_string (l.v_lex) + ", " + to_string (l.t_lex)
          + ") in line - " + to_string (l.str_nom) + " in column - " + to_string (l.stb_nom) + "\n");
    return s;
}




