#include "Execution.hpp"

void Executer::execute ( vector<Lex> & poliz ) {
    cout << "EXECUTION:" << endl << endl;
    Lex pc_el;
    stack < object > args;
    int i, j, index = 0, size = poliz.size();
    object temp1( other, 0);
    object temp2( other, 0);
    int k;
    string tmp_string;
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_TRUE: case LEX_FALSE:
                args.push ( object (ident, pc_el.get_value ()) );
                break;
            case LEX_NUM: case POLIZ_LABEL:
                args.push ( object (integer, pc_el.get_value ()) );
                break;
            case POLIZ_ADDRESS:
                args.push ( object (ident, pc_el.get_value ()) );
                break;
            case LEX_STRING:
                args.push ( object (str, STR_TABLE [pc_el.get_value ()]) );
                break;
            case LEX_ID:
                temp1.obj_int = pc_el.get_value ();
                if (TID[pc_el.get_value()].get_assign()) {
                    int lex_type = 0;
                    lex_type = TID[pc_el.get_value()].get_type();
                    if (lex_type == LEX_STRING) {
                        temp1 = object (str, STR_TABLE [ TID[pc_el.get_value()].get_value() ]);
                    } else if (lex_type == LEX_BOOL) {
                        temp1 = object (boolean, TID[pc_el.get_value()].get_value() );
                    } else if (lex_type == LEX_INT) {
                        temp1 = object (integer, TID[pc_el.get_value()].get_value() );
                    }
                    args.push(temp1);
                } else {
                    throw "POLIZ: indefinite identifier";
                }
                break;
            case LEX_NOT:
                from_st ( args, temp1 );
                args.push (object (boolean, !temp1.obj_int));
                break;
            case LEX_OR:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                args.push (object (boolean, temp1.obj_int || temp2.obj_int));
                break;
            case LEX_AND:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                args.push (object (boolean, temp1.obj_int && temp2.obj_int));
                break;
            case POLIZ_GO:
                from_st ( args, temp1 );
                index = temp1.obj_int - 1;
                break;
            case POLIZ_FGO:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if ( !temp2.obj_int ) index = temp1.obj_int - 1;
                break;
            case LEX_WRITE: {
                from_st ( args, temp2 );
                stack < object > reverse_args;
                int p = temp2.obj_int;
                while (temp2.obj_int > 0) {
                    from_st ( args, temp1 );
                    reverse_args.push (temp1);
                    temp2.obj_int = temp2.obj_int - 1;
                }
                while (p > 0) {
                    from_st ( reverse_args, temp1 );
                    if (temp1.obj_type == ident) {
                        if (temp1.obj_type == str) {
                            cout << STR_TABLE [TID[temp1.obj_int].get_value()];
                        } else if (temp1.obj_type == boolean) {
                            if (TID [temp1.obj_int].get_value()) cout << "true";
                            else cout << "false";
                        } else if (temp1.obj_type == integer) cout << TID[temp1.obj_int].get_value();
                    } else if (temp1.obj_type == str) {
                            cout << temp1.obj_str;
                    } else if (temp1.obj_type == boolean) {
                        if (temp1.obj_int) cout << "true";
                        else cout << "false";
                    }
                    else if (temp1.obj_type == integer) cout << temp1.obj_int;
                    p--;
                }
                cout << endl;
            }
                break;
            case LEX_READ:
                from_st ( args, temp1 );
                i = temp1.obj_int;
                if ( TID[i].get_type () == LEX_INT ) {
                    cout << "Input int value for " << TID[i].get_name () << endl;
                    cin >> k;
                    TID[i].put_value (k);
                    TID[i].put_assign ();
                }
                else if (TID[i].get_type() == LEX_STRING) {
                    cout << "Input string value for" << TID[i].get_name () << endl;
                    cin >> tmp_string;
                    STR_TABLE [TID[i].get_value()] = tmp_string;
                    TID[i].put_assign ();
                } else {
                    string j;
                    while (1) {
                        cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
                        cin >> j;
                        if ( j != "true" && j != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( j == "true" ) ? 1 : 0;
                        break;
                    }
                    TID[i].put_value (k);
                    TID[i].put_assign ();
                    }
                break;
            case LEX_PLUS:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_type == str) {
                    temp1.obj_str = temp1.obj_str;
                    temp2.obj_str = temp2.obj_str;
                    args.push ( object (str, temp1.obj_str + temp2.obj_str) );
                }
                else {
                    temp1.obj_int = temp1.obj_int;
                    temp2.obj_int = temp2.obj_int;
                    args.push ( object (integer, temp1.obj_int + temp2.obj_int) );
                }
                break;
            case LEX_TIMES:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                args.push ( object (integer, temp1.obj_int * temp2.obj_int) );
                break;
            case LEX_MINUS:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                args.push ( object (integer, temp2.obj_int - temp1.obj_int) );
                break;
            case LEX_SLASH:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (! ((temp1.obj_int == 0)   )) {
                    args.push (object (integer, temp2.obj_int / temp1.obj_int ));
                    break;
                } else
                    throw "POLIZ :divide by zero";
                break;
            case LEX_PERCENT:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_int == 0 || temp1.obj_int == 1) throw "POLIZ: bad %";
                args.push ( object (integer,temp2.obj_int % temp1.obj_int) );
                break;
            case LEX_EQ:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_type == str)
                    args.push ( object (str, temp1.obj_str == temp2.obj_str) );
                else
                    args.push ( object (integer, temp1.obj_int == temp2.obj_int) );
                break;
            case LEX_LSS:
                from_st ( args, temp1 );
                from_st ( args, temp2 );

                if (temp1.obj_type == str)
                    args.push ( object (str, temp2.obj_str < temp1.obj_str) );
                else
                    args.push ( object (integer, temp2.obj_int < temp1.obj_int) );
                break;
            case LEX_GTR:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_type == str)
                    args.push ( object (str, temp2.obj_str > temp1.obj_str) );
                else
                    args.push ( object (integer, temp2.obj_int > temp1.obj_int) );
                break;
            case LEX_LEQ:
                from_st ( args, temp1 );
                from_st ( args, temp2 );

                if (temp1.obj_type == str)
                    args.push ( object (str, temp2.obj_str <= temp1.obj_str) );
                else
                    args.push ( object (integer, temp2.obj_int <= temp1.obj_int) );
                break;
            case LEX_GEQ:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_type == str)
                    args.push ( object (str, temp2.obj_str >= temp1.obj_str) );
                else
                    args.push ( object (integer, temp2.obj_int >= temp1.obj_int) );
                break;
            case LEX_NEQ:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_type == str)
                    args.push ( object (str, temp1.obj_str != temp2.obj_str) );
                else
                    args.push ( object (integer, temp1.obj_int != temp2.obj_int) );
                break;
            case LEX_ASSIGN:
                from_st ( args, temp1 );
                from_st ( args, temp2 );
                if (temp1.obj_type == str) {
                    if (temp1.obj_type == ident) {
                        STR_TABLE [TID[temp2.obj_int].get_value()] = STR_TABLE [TID[temp1.obj_int].get_value()];
                    } else {
                        STR_TABLE [TID[temp2.obj_int].get_value()] = temp1.obj_str;
                    }
                    TID[temp2.obj_int].put_assign ();
                    args.push (temp2);
                } else {
                    if (temp1.obj_type == ident) {
                        TID [temp2.obj_int].put_value(TID [temp1.obj_int].get_value());
                    } else {
                        TID[temp2.obj_int].put_value (temp1.obj_int);
                    }
                    TID[temp2.obj_int].put_assign ();
                    args.push (temp2);
                }
                break;
            case LEX_SEMICOLON:
                if (!args.empty()) from_st ( args, temp1 );
                break;
            default:
                throw "POLIZ: unexpected elem";
            }
        ++index;
        };
    cout << endl << "EXECUTION DONE!" << endl;
    }
