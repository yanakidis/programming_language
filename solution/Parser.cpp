#include "Parser.hpp"

void Parser::analyze () {
    gl ();
    P ();
    gl ();
    if (c_type != LEX_END) {
        exc_str = "Inappropriate ending of the program.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
    cout << endl << "SYNTAX DONE!" << endl << endl;
    
    for ( Lex l : poliz )
        cout << l;
    cout << endl << "POLIZ DONE!" << endl << endl;
}

void Parser::P () {
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    } else {
        exc_str = "Inappropriate beginning of the program.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
    if ( c_type == LEX_LBRACKET ) {
        gl ();
    } else {
        exc_str = "Inappropriate beginning of the program.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
    D1 ();
    S ();
    while (c_type != LEX_RBRACKET) {
        S();
    } if ( c_type == LEX_RBRACKET ) {
        gl ();
    } else {
        exc_str = "Inappropriate ending of the program.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
}

void Parser::D1 () {
    while ( (c_type == LEX_INT) || (c_type == LEX_BOOL) || (c_type == LEX_STRING)) {
        D ();
        if (c_type == LEX_SEMICOLON) {
            gl ();
        } else {
            exc_str = "Inappropriate ending of the declaration.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
    }
}

void Parser::D () {
    type_of_lex temp_type = c_type;
    int identy  = 0;
    int i = 0;
    gl ();
    if ( c_type != LEX_ID ) {
        exc_str = "Inappropriate declarationa.\n";
        exc_str << curr_lex;
        throw exc_str;
    } else {
        if ( TID[c_val].get_declare () ) {
            exc_str = "Double declaration.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        identy = c_val;
        TID[identy].put_declare ();
        TID[identy].put_type (temp_type);
        gl ();
        if (c_type == LEX_ASSIGN) {
            gl ();
            if (c_type == LEX_NUM && TID[identy].get_type() != LEX_INT) {
                exc_str = "Wrong declaration.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (c_type == LEX_STRING && TID[identy].get_type() != LEX_STRING) {
                exc_str = "Wrong declaration.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if ((c_type == LEX_TRUE || c_type == LEX_FALSE) && (TID[identy].get_type() != LEX_BOOL)) {
                exc_str = "Wrong declaration.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (c_type == LEX_NUM) TID[identy].put_value (c_val);
            else if (c_type == LEX_STRING) TID[identy].put_value (c_val);
            else if (c_type == LEX_TRUE || c_type == LEX_FALSE) TID[identy].put_value (c_val);
            else {
                exc_str = "Wrong declaration.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            TID[identy].put_assign();
            gl();
        }
        while ( c_type == LEX_COMMA ) {
            gl ();
            if ( c_type != LEX_ID ) {
                exc_str = "Wrong declaration.\n";
                exc_str << curr_lex;
                throw exc_str;
            } else {
                if ( TID[c_val].get_declare () ) {
                    exc_str = "Double declaration.\n";
                    exc_str << curr_lex;
                    throw exc_str;
                }
                identy = c_val;
                TID[identy].put_declare ();
                TID[identy].put_type ( temp_type );
                gl ();
                if (c_type == LEX_ASSIGN) {
                    gl ();
                    if (c_type == LEX_NUM && TID[identy].get_type() != LEX_INT) {
                        exc_str = "Wrong declaration in ";
                        exc_str << curr_lex;
                        throw exc_str;
                    }
                    if (c_type == LEX_STRING && TID[identy].get_type() != LEX_STRING){
                        exc_str = "Wrong declaration in ";
                        exc_str << curr_lex;
                        throw exc_str;
                    }
                    if ((c_type == LEX_TRUE || c_type == LEX_FALSE) && (TID[identy].get_type() != LEX_BOOL)) {
                        exc_str = "Wrong bool declaration in ";
                        exc_str << curr_lex;
                        throw exc_str;
                    }
                    if (c_type == LEX_NUM) TID[identy].put_value (c_val);
                    else if (c_type == LEX_STRING) TID[identy].put_value (c_val);
                    else if (c_type == LEX_TRUE || c_type == LEX_FALSE) TID[identy].put_value (c_val);
                    else {
                        exc_str = "Wrong declaration in ";
                        exc_str << curr_lex;
                        throw exc_str;
                    }
                    TID[identy].put_assign();
                    gl();
                }
            }
        }
    }
}

void Parser::S () {
    int pl0, pl1, pl2, pl3;
    if ( c_type == LEX_IF ) {
        gl();
        if (c_type != LEX_LPAREN) {
            exc_str = "Inappropriate IF. ( required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl ();
        E1 ();
        eq_bool ();
        pl2 = poliz.size();
        poliz.push_back ( Lex(LEX_NULL, 0, c_str_nom, c_stb_nom ));
        poliz.push_back ( Lex(POLIZ_FGO, 0, c_str_nom, c_stb_nom ));
        if (c_type != LEX_RPAREN) {
            exc_str = "Inappropriate IF. ) required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl();
        S();
        if(c_type == LEX_SEMICOLON) gl();
        if (c_type == LEX_ELSE) {
            pl3 = poliz.size ();
            poliz.push_back ( Lex (LEX_NULL, 0, c_str_nom, c_stb_nom ));
            poliz.push_back ( Lex ( POLIZ_GO, 0, c_str_nom, c_stb_nom  ));
            poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size(), c_str_nom, c_stb_nom );
            gl ();
            S ();
            poliz[pl3] = Lex ( POLIZ_LABEL, poliz.size(), c_str_nom, c_stb_nom  );
            int tmp_size = poliz.size();
        }
        else {
            poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );
            int tmp_size = poliz.size();
        }
    } else if ( c_type == LEX_WHILE ) {
        pl0 = poliz.size ();
        gl ();
        if (c_type != LEX_LPAREN) {
            exc_str = "Inappropriate WHILE . ( required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl ();
        E1 ();
        eq_bool ();
        if (c_type != LEX_RPAREN) {
            exc_str = "Inappropriate IF. ) required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl ();
        pl1 = poliz.size ();
        poliz.push_back ( Lex (LEX_NULL, 0, c_str_nom, c_stb_nom ));
        poliz.push_back ( Lex (POLIZ_FGO, 0, c_str_nom, c_stb_nom ) );
        S();
        poliz.push_back ( Lex ( POLIZ_LABEL, pl0, c_str_nom, c_stb_nom ) );
        poliz.push_back ( Lex ( POLIZ_GO, 0, c_str_nom, c_stb_nom ));
        poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size(), c_str_nom, c_stb_nom );
        int tmp_size = poliz.size();
        for (int i = pl0; i < tmp_size; i++) {
            cout << i << " ";
            cout << poliz[i];
            if (poliz[i].get_type() == LEX_BREAK) {
                poliz[i] = Lex (POLIZ_LABEL, tmp_size, c_str_nom, c_stb_nom);
                cout << poliz[i].get_value() << endl;
            }
        }
    } else if (c_type == LEX_DO) {
        pl0 = poliz.size ();
        gl ();
        S();
        if (c_type != LEX_WHILE) {
            exc_str = "Inappropriate DO WHILE.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl();
        if (c_type != LEX_LPAREN) {
            exc_str = "Inappropriate DO WHILE. ( required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl ();
        E1();
        if (c_type != LEX_RPAREN) {
            exc_str = "Inappropriate DO WHILE bracketing. ) required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl ();
        poliz.push_back ( Lex ( POLIZ_LABEL, poliz.size() + 4, c_str_nom, c_stb_nom ) );
        poliz.push_back ( Lex ( POLIZ_FGO, 0, c_str_nom, c_stb_nom ));
        poliz.push_back ( Lex ( POLIZ_LABEL, pl0, c_str_nom ) );
        poliz.push_back ( Lex ( POLIZ_GO, 0, c_str_nom, c_stb_nom ));
        if (c_type != LEX_SEMICOLON) {
            exc_str = "Inappropriate DO WHILE bracketing. ) required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl();
        eq_bool();
    } else if (c_type == LEX_BREAK) {
        poliz.push_back ( Lex (LEX_BREAK, 0, c_str_nom, c_stb_nom ));
        poliz.push_back ( Lex (POLIZ_GO, 0, c_str_nom, c_stb_nom ));
        gl();
        if (c_type == LEX_SEMICOLON) gl();
    } else if ( c_type == LEX_READ ) {
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            if ( c_type == LEX_ID ) {
                check_id_in_read ();
                poliz.push_back ( Lex( POLIZ_ADDRESS, c_val, c_str_nom, c_stb_nom) );
                gl();
            } else {
                exc_str = "Inappropriate READ. IDENT required.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex (LEX_READ, 0, c_str_nom, c_stb_nom ));
            } else {
                exc_str = "Inappropriate READ. ) required.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
        } else {
            exc_str = "Inappropriate READ. ( required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        if (c_type == LEX_SEMICOLON) gl();
    } else if ( c_type == LEX_WRITE ) {
        int number_to_be_written = 0;
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            E1 ();
            number_to_be_written++;
            while ( c_type == LEX_COMMA ) {
                gl();
                E1();
                number_to_be_written++;
            }
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex ( LEX_NUM, number_to_be_written, c_str_nom, c_stb_nom ));
                poliz.push_back ( Lex ( LEX_WRITE, 0, c_str_nom, c_stb_nom ) );
            } else {
                exc_str = "Inappropriate WRITE. ) required.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (c_type == LEX_SEMICOLON) gl();
        } else {
            exc_str = "Inappropriate WRITE. ( required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
    } else if (c_type == LEX_LBRACKET) {
            pl0 = poliz.size();
            gl();
            do {
                S ();;
                if (c_type == LEX_SEMICOLON) gl();
            } while (c_type != LEX_RBRACKET);
            gl();
        } else {
        E1 ();
        if (c_type == LEX_SEMICOLON) gl();
        poliz.push_back ( Lex ( LEX_SEMICOLON, 0, c_str_nom, c_stb_nom ) );
        }
}

void Parser::E1 () {
    E2();
    while (c_type == LEX_ASSIGN) {
        if (poliz [poliz.size() - 1].get_type() == POLIZ_ADDRESS || poliz [poliz.size() - 1].get_type() == LEX_ID || poliz [poliz.size() - 1].get_type() == LEX_ASSIGN)
            poliz [poliz.size() - 1] = Lex (POLIZ_ADDRESS, poliz [poliz.size() - 1].get_value());
        else {
            exc_str = "Inappropriate assignment.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
        gl();
        st_lex.push(LEX_ASSIGN);
        E2();
        check_op();
    }
}

void Parser::E2 () {
    E3();
    while ( c_type == LEX_OR) {
        gl();
        st_lex.push(LEX_OR);
        E3();
        check_op();
    }
}

void Parser::E3 () {
    E4();
    while ( c_type == LEX_AND) {
        gl();
        st_lex.push(LEX_AND);
        E4();
        check_op();
    }
}

void Parser::E4 () {
    E5();
    while ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push ( c_type );
        gl ();
        E5 ();
        check_op ();
    }
}

void Parser::E5 () {
    E6();
    while ( c_type == LEX_PLUS  || c_type == LEX_MINUS ) {
        st_lex.push ( c_type );
        gl ();
        E6 ();
        check_op ();
    }
}

void Parser::E6 () {
    E7();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_PERCENT) {
        st_lex.push ( c_type );
        gl ();
        E7 ();
        check_op ();
    }
}

void Parser::E7 () {
    if ( c_type == LEX_ID ) {
        check_id ();
        poliz.push_back ( Lex ( LEX_ID, c_val, c_str_nom, c_stb_nom  ) );
        gl ();
    } else if ( c_type == LEX_NUM ) {
        st_lex.push ( LEX_INT );
        poliz.push_back ( Lex ( c_type , c_val,  c_str_nom, c_stb_nom ) );
        gl ();
    } else if ( c_type == LEX_STRING ) {
        st_lex.push ( LEX_STRING );
        poliz.push_back ( Lex ( c_type , c_val,  c_str_nom, c_stb_nom ) );
        gl ();
    } else if ( c_type == LEX_TRUE ) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_TRUE, 1,  c_str_nom, c_stb_nom) );
        gl ();
    } else if ( c_type == LEX_FALSE) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_FALSE, 0,  c_str_nom, c_stb_nom) );
        gl ();
    } else if ( c_type == LEX_NOT ) {
        gl ();
        E7 ();
        check_not ();
    } else if ( c_type == LEX_LPAREN ) {
        gl ();
        E1 ();
        if ( c_type == LEX_RPAREN) gl ();
        else {
            exc_str = "Inappropriate (). ) required.\n";
            exc_str << curr_lex;
            throw exc_str;
        }
    } else {
        exc_str = "Inappropriate symbol.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
}

void Parser::check_id () {
    if ( TID[c_val].get_declare() )
        {
        st_lex.push ( TID[c_val].get_type () );
        st_lex.push (LEX_ID);
    } else {
        exc_str = "Undeclared IDENT.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
}

void Parser::check_op () {
    type_of_lex t1 = LEX_NULL, t2 = LEX_NULL, op = LEX_NULL;
    type_of_lex t1_real = LEX_NULL, t2_real = LEX_NULL;
    from_st ( st_lex, t1 );
    if (t1 == LEX_ID) from_st (st_lex, t1_real);
    from_st ( st_lex, op );
    from_st ( st_lex, t2 );
    if (t2 == LEX_ID) from_st (st_lex, t2_real);
    switch (op) {
        case LEX_ASSIGN: {
            if (t2_real == t1 || t2_real == t1_real) {
                st_lex.push (t2_real);
                st_lex.push (t2);
            } else {
                exc_str = "Wrond assignment, left part is not assignablet.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            break;
        }
        case LEX_LEQ: case LEX_GEQ: case LEX_NEQ: case LEX_EQ: case LEX_LSS: case LEX_GTR:{
            type_of_lex tmp1 = LEX_NULL;
            type_of_lex tmp2 = LEX_NULL;
            if (t1_real) tmp1 = t1_real;
            else tmp1 = t1;
            if (t2_real) tmp2 = t2_real;
            else tmp2 = t2;
            if (tmp1 == LEX_INT ) tmp1 = LEX_INT;
            else if (tmp1 == LEX_STRING) tmp1 = LEX_STRING;
            else if (tmp1 == LEX_NUM ) tmp1 = LEX_INT;
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (tmp2 == LEX_INT ) tmp2 = LEX_INT;
            else if (tmp2 == LEX_STRING) tmp2 = LEX_STRING;
            else if (tmp2 == LEX_NUM ) tmp2 = LEX_INT;
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (tmp1 == tmp2) st_lex.push (LEX_BOOL);
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            break;
        }
        case LEX_AND: case LEX_OR: {
            if (!(t1 == LEX_BOOL || t1_real == LEX_BOOL )) { //tmp1 = BOOL;
                exc_str = "Wrond OR.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (!(t2 == LEX_BOOL || t2_real == LEX_BOOL )) { //tmp2 = BOOL;
                exc_str = "Wrong OR.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (t2_real) st_lex.push (t2_real);
            else if (t2) st_lex.push (t2);
            break;
        }
        case LEX_SLASH: case LEX_PERCENT: case LEX_MINUS: case LEX_TIMES: {
            type_of_lex tmp1 = LEX_NULL;
            type_of_lex tmp2 = LEX_NULL;
            if (t1_real) tmp1 = t1_real;
            else tmp1 = t1;
            if (t2_real) tmp2 = t2_real;
            else tmp2 = t2;
            if (tmp1 == LEX_INT ) tmp1 = LEX_INT;
            else if (tmp1 == LEX_NUM ) tmp1 = LEX_INT;
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (tmp2 == LEX_INT ) tmp2 = LEX_INT;
            else if (tmp2 == LEX_NUM ) tmp2 = LEX_INT;
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (tmp1 == tmp2) st_lex.push (LEX_INT);
            else {
                exc_str = "Not integer.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            break;
        }
        case LEX_PLUS: {
            type_of_lex tmp1 = LEX_NULL;
            type_of_lex tmp2 = LEX_NULL;
            if (t1_real) tmp1 = t1_real;
            else tmp1 = t1;
            if (t2_real) tmp2 = t2_real;
            else tmp2 = t2;
            if  (tmp1 == LEX_INT ) tmp1 = LEX_INT;
            else if (tmp1 == LEX_STRING) tmp1 = LEX_STRING;
            else if (tmp1 == LEX_NUM ) tmp1 = LEX_INT;
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (tmp2 == LEX_INT ) tmp2 = LEX_INT;
            else if (tmp2 == LEX_STRING) tmp2 = LEX_STRING;
            else if (tmp2 == LEX_NUM ) tmp2 = LEX_INT;
            else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            if (tmp1 == tmp2) {
                if (tmp1 == LEX_STRING) st_lex.push (LEX_STRING);
                else if (tmp1 == LEX_INT) st_lex.push (LEX_INT);
            } else {
                exc_str = "Wrong second argument.\n";
                exc_str << curr_lex;
                throw exc_str;
            }
            break;
        }
        default:
            exc_str = "Unknown operation.\n";
            exc_str << curr_lex;
            throw exc_str;
    }
    //cout << "POLIZ OPERATION" << Lex(op) << endl;
    poliz.push_back (Lex (op, 0,  c_str_nom, c_stb_nom));
}

void Parser::check_not () {
    type_of_lex t1 = LEX_NULL;
    type_of_lex t1_real = LEX_NULL;
    from_st ( st_lex, t1 );
    if (t1 == LEX_ID) from_st (st_lex, t1_real);
    if (!(t1 == LEX_BOOL || t1_real == LEX_BOOL )) { //tmp1 = BOOL
        exc_str = "Not boolean type.\n";
        exc_str << curr_lex;
        throw exc_str;
    } else {
        poliz.push_back ( Lex (LEX_NOT, 0, c_str_nom, c_stb_nom ));
        st_lex.push (LEX_BOOL);
    }
}

void Parser::eq_bool () {
    int tmp_type = st_lex.top();
    if ( tmp_type != LEX_BOOL ) {
        cout << tmp_type << endl;
        exc_str = "Not boolean.\n";
        exc_str << curr_lex;
        throw exc_str;
    }
    st_lex.pop ();
}

void Parser::check_id_in_read () {
    if ( !TID [c_val].get_declare() ) {
        exc_str = "Undeclared IDENT.\n";
        exc_str += TID[c_val].get_name();
        exc_str << curr_lex;
        throw exc_str;
    }
}
