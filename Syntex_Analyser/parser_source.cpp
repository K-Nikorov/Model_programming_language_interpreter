#include "parser_header.h"
#include "../common.h"

extern Table_Ident TID;
extern Table_Const_Str TCS;
extern Table_Struct TS;

void Parser::start_cycle() {++in_cycle;}
void Parser::finish_cycle() {--in_cycle;}
bool  Parser::is_in_cycle() { return in_cycle>0;}

void Parser::forbid_descr() {descr_permit = false;}
void Parser::forbid_struc() {struc_permit = false;}
void Parser::copy_struct(int to, int from) { //TODO: have problem with get_value!!!!
    Struct s;
    int k;
    //cout << "Copying now\n";
    //cout << TS[TID[from].get_value()].get_top() << endl;
    for (int j=0; j < TS[TID[from].get_value()].get_top(); ++j) {
        Ident* id = TS[TID[from].get_value()][j];
        Ident new_f;
        new_f.put_name(id->get_name());
        new_f.remark_name(to);
        k = TID.put(new_f.get_name());
        /*cout << "Type: " << from << "--" << "Num: " << k << endl;
        cout << "Name: " << TID[k].get_name() << endl;*/
        TID[k].put_declare();
        TID[k].put_type(id->get_type());
        TID[k].put_assign();
        if (id->get_type() == LEX_TYPE_STRING) {
            TID[k].put_value(TCS.put(TCS[id->get_value()].c_str()));
        } else if (id->get_type() > LEX_LAST_IN_LIST) {
            copy_struct(k, id->get_type()-LEX_LAST_IN_LIST);
        } else {
            TID[k].put_value(id->get_value());
        }
        s.add_field(&TID[k]);
    }
    k = TS.put(s);
    /*cout << TS[k][0]->get_name() << endl;
    cout << TS[TID[from].get_value()][0]->get_name() << endl;*/
    /*for (int j=0; j < TS[k].get_top(); ++j) {
        cout << TS[k][j] << endl;
    }*/
    TID[to].put_value(k);
    /*cout << "New value: " << k << " -- Old value: " << TID[from].get_value() << endl;
    cout << "Content : " << TS[TID[to].get_value()][0]->get_value() << endl;*/
}
void Parser::get_in_struc() {++in_struc;}
void Parser::out_of_struc() {--in_struc;}
bool Parser::is_in_struc() {return in_struc>0;}
void Parser::gl() {
    curr_lex = scan.get_lex();
    c_type = curr_lex.get_type();
    c_val = curr_lex.get_val();
    //cout << curr_lex;
    //if (c_type == LEX_ID) cout << TID[c_val].get_name() << "_; ";
}
void Parser::parse_prog(const char* program) {
    scan.scan_prog(program);
}
void Parser::close_prog(const char* program) {
    scan.close_prog(program);
}


void Parser::check_id() {
    if (TID[c_val].get_declare()) {
        if (TID[c_val].is_var())
            st_lex.push(TID[c_val].get_type());
        else throw "label can't be used in expression!";
    } else throw "not declared!";
}

void Parser::check_op() {
    int t1, op, t2, t, r;
    t2 = st_lex.top(); st_lex.pop();
    op = st_lex.top(); st_lex.pop();
    t1 = st_lex.top(); st_lex.pop();
    if (t1 != t2) {
        if (!((t1 == LEX_TYPE_INT && t2==LEX_NUM) || (t1 == LEX_TYPE_STRING && t2 == LEX_CONST_STR) || ((t1 == LEX_TYPE_BOOL) && (t2 == LEX_TRUE || t2 == LEX_FALSE)) || (t1 == LEX_TRUE && t2 == LEX_FALSE) || (t1 == LEX_FALSE && t2 == LEX_FALSE)))
        {
            throw "wrong types in operation";
        }
    }
    if (op == LEX_ASSIGN) {
      if (t1 == LEX_TYPE_STRING) {
        op = LEX_STRING_ASSIGN;
        t = t1;
        r = t1;
      } else if (t1 > LEX_LAST_IN_LIST){
          op = LEX_STRUCT_ASSIGN;
          t = t1;
          r = t1;
      } else if (t1 == LEX_TRUE || t1 == LEX_FALSE) {
        t = LEX_TYPE_BOOL;
        r = LEX_TYPE_BOOL;
      } else {
          t = t1;
          r = t1;
      }
    } else if (op == LEX_PLUS) {
        if (t1 == LEX_TYPE_STRING) {
            r = LEX_TYPE_STRING;
            t = LEX_TYPE_STRING;
            op = LEX_CONCAT;
        }
        else if (t1 == LEX_TYPE_INT) {
            r = LEX_TYPE_INT;
            t = LEX_TYPE_INT;
        }
        else throw "\nwrong types in operation";
    } else if (op == LEX_MINUS || op == LEX_TIMES || op == LEX_DIVIDE) {
        r = LEX_TYPE_INT;
        t = LEX_TYPE_INT;
    } else if (op >= LEX_EQ && op <= LEX_NOTEQ) {
        if (t1 == LEX_TYPE_STRING) {
            if (op == LEX_EQ)
                op = LEX_STRING_EQ;
            else if (op == LEX_NOTEQ)
                op = LEX_STRING_NOTEQ;
            else if (op == LEX_LSS)
                op = LEX_STRING_LSS;
            else if (op == LEX_LSEQ)
                op = LEX_STRING_LSEQ;
            else if (op == LEX_GRT)
                op = LEX_STRING_GRT;
            else if (op == LEX_GTEQ)
                op = LEX_STRING_GTEQ;
        }
        if (t1 == LEX_TYPE_BOOL) throw "\nwrong types in operation";
        r = LEX_TYPE_BOOL;
        t = t1;
    } else if (op == LEX_OR || op == LEX_AND) {
        r = LEX_TYPE_BOOL;
        t = LEX_TYPE_BOOL;
    }
    if (t2 != t) throw "\nwrong types in operation";
    st_lex.push(r);
    prog.put_lex(Lex(op));
}

void Parser::check_not() {
    if (st_lex.top() != LEX_TYPE_BOOL)
        throw "\nwrong type in not";
    else {
        st_lex.pop();
        st_lex.push(LEX_TYPE_BOOL);
        prog.put_lex(Lex(LEX_NOT));
    }
}

void Parser::eq_bool() {
    if (st_lex.top() != LEX_TYPE_BOOL) throw "non-bool type in if-expression!" ;
    st_lex.pop();
}

void Parser::check_id_in_read() {
    if (!TID[c_val].is_var()) throw "label is forbidden in read argument!";
    if (!TID[c_val].get_declare())  throw "read argument is not declared!";
    //if (TID[c_val].get_type() == LEX_TYPE_BOOL) throw "bool-variable can't be read argument!";
}

void Parser::check_id_in_for() {
    if (st_lex.top() != LEX_TYPE_INT) throw "wrong parameter in for-cycle!";
}

void Parser::analyze(const char* program){
    parse_prog(program);
    gl();
    P();
    cout << "\nFinish syntax!\n";
    cout << "\nPoliz: \n";
    prog.print();
    close_prog(program);
}

void Parser::P() {
    if (c_type == LEX_PROGRAM) gl();
    else throw curr_lex;
    if (c_type != LEX_FIG_OPEN) throw curr_lex;
    else gl();
    Ds(false);
    forbid_descr();
    Os();
    if (c_type != LEX_FIG_CLOSE) throw curr_lex;
    gl();
    if (c_type != LEX_FIN) throw "end of prog expected\n";
    for (auto i: v_labels) {
        if (i) throw "some labels are not assigned!";
    }
}

void Parser::Ds(bool in_struc) {
    if (c_type == LEX_STRUCT) {
        if (!struc_permit) throw "can't define structs after variables!";
        if (in_struc) throw "defining struc inside other struc!";
        gl();
        V_struc(in_struc);
        //gl();
        Ds(in_struc);
    } else if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
        if (!in_struc && struc_permit) forbid_struc();
        D(in_struc);
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
        Ds(in_struc);
    } else if (c_type == LEX_ID && TID[c_val].get_declare() && TID[c_val].get_type() == LEX_TYPE_STRUCT) {
        cout << "Actions for struct don't complete!\n";
        last_struc = c_val;
        D(in_struc);
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
        last_struc = 0;
        Ds(in_struc);
    }
}

void Parser::D(bool in_struc) {
    //сейчас в лексеме - тип переменной!
    if (c_type == LEX_INT) {
        gl();
        V_int(in_struc);
    } else if (c_type == LEX_BOOL) {
        gl();
        V_bool(in_struc);
    } else if (c_type == LEX_STRING) {
        gl();
        V_str(in_struc);
    } else if (c_type == LEX_ID) {
        gl();
        V_comp_type(in_struc);
    }
}

void Parser::V_comp_type(bool in_struc) {
    if (c_type != LEX_ID) throw curr_lex;
    if (in_struc) {
        TID[c_val].remark_name(st_struc.top());
        st_fields.push(&TID[c_val]);
    }
    if ( !TID[c_val].get_declare() ) {
        TID[c_val].put_type(last_struc+LEX_LAST_IN_LIST);
        TID[c_val].put_declare();
        //cout << "Name of struct: " << TID[c_val].get_name() << endl;
        copy_struct(c_val, last_struc);
        TID[c_val].put_assign();
        /*prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
        prog.put_lex(Lex(LEX_ID, last_struc));
        prog.put_lex(Lex(LEX_ASSIGN));*/
        gl();
        if (c_type == LEX_COMA) {
            gl();
            V_comp_type(in_struc);
        }
    } else throw "declared twice!";
}

void Parser::V_int(bool in_struc) {
    if (c_type == LEX_ID) {
        if (in_struc) {
            TID[c_val].remark_name(st_struc.top());
            st_fields.push(&TID[c_val]);
        }
        if ( !TID[c_val].get_declare() ) {
            Lex temp = curr_lex;
            TID[c_val].put_type(LEX_TYPE_INT);
            TID[c_val].put_declare();
            st_lex.push(TID[c_val].get_type());
            gl();
            if (c_type == LEX_ASSIGN) {
                prog.put_lex(Lex(POLIZ_ADDRESS, temp.get_val()));
                st_lex.push(LEX_ASSIGN);
                gl();
                E_as();
                check_op();
                prog.put_lex(Lex(POLIZ_DELETE));
                TID[c_val].put_assign();
            } else {
                st_lex.pop();
            }
            if (c_type == LEX_COMA) {
                gl();
                V_int(in_struc);
            }
        } else throw "declared twice!";
    } else throw curr_lex;
}

void Parser::V_bool(bool in_struc) {
    if (c_type == LEX_ID) {
        if (in_struc) {
            TID[c_val].remark_name(st_struc.top());
            st_fields.push(&TID[c_val]);
        }
        if ( !TID[c_val].get_declare() ) {
            Lex temp = curr_lex;
            TID[c_val].put_type(LEX_TYPE_BOOL);
            TID[c_val].put_declare();
            st_lex.push(TID[c_val].get_type());
            gl();
            if (c_type == LEX_ASSIGN) {
                prog.put_lex(Lex(POLIZ_ADDRESS, temp.get_val()));
                st_lex.push(LEX_ASSIGN);
                gl();
                E_as();
                check_op();
                prog.put_lex(Lex(POLIZ_DELETE));
                TID[c_val].put_assign();
            } else {
                st_lex.pop();
            }
            if (c_type == LEX_COMA) {
                gl();
                V_bool(in_struc);
            }
        } else throw "declared twice!";
    } else throw curr_lex;
}

void Parser::V_str(bool in_struc) {
    if (c_type != LEX_ID) throw curr_lex;
    if (in_struc) {
        TID[c_val].remark_name(st_struc.top());
        st_fields.push(&TID[c_val]);
    }
    if ( TID[c_val].get_declare() ) throw "declared twice!";
    Lex temp = curr_lex;
    TID[c_val].put_type(LEX_TYPE_STRING);
    TID[c_val].put_declare();
    st_lex.push(TID[c_val].get_type());
    gl();
    if (c_type == LEX_ASSIGN) {
        prog.put_lex(Lex(POLIZ_ADDRESS, temp.get_val()));
        st_lex.push(LEX_ASSIGN);
        gl();
        E_as();
        check_op();
        prog.put_lex((Lex(POLIZ_DELETE)));
        TID[temp.get_val()].put_assign();
    } else {
        st_lex.pop();
    }
    TID[temp.get_val()].put_value(TCS.put("\0"));
    if (c_type == LEX_COMA) {
        gl();
        V_str(in_struc);
    }
}

void Parser::V_struc(bool in_struc) {
    if (c_type != LEX_ID) throw "struct identifier expected!";
    if ( !TID[c_val].get_declare() ) {
            TID[c_val].put_type(LEX_TYPE_STRUCT);
            TID[c_val].put_declare();
            st_struc.push(c_val);
    } else throw "declared twice!";
    int temp = c_val;
    gl();
    if (c_type != LEX_FIG_OPEN) throw "'{' expected!";

    gl();
    Ds(true);
    if (c_type == LEX_FIG_CLOSE) {
        gl();
        if (c_type != LEX_SEMICOLON) throw "';' expected!";
        gl();
    } else throw "wrong struct description!";
    st_struc.pop();
    Struct s;
    while (!st_fields.empty()) {
        s.add_field(st_fields.top());
        st_fields.pop();
    }
    int kk = TS.put(s);
    TID[temp].put_value(kk);
}



void Parser:: Os() {
    while (c_type != LEX_FIG_CLOSE) {
        if (c_type == LEX_FIN) throw "\nerror: unexpected end of program!";
        O();
    }
}

void Parser::O() {
    int pl2, pl3;
    if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING || c_type == LEX_STRUCT) throw "descriptions are unavailable in operators scope!";
    if (c_type == LEX_IF) {
        gl();
        if (c_type != LEX_BRAC_OPEN) throw curr_lex;
        gl();
        E_as();
        eq_bool();
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type != LEX_BRAC_CLOSE) throw curr_lex;
        gl();
        O();
        pl3 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
        if (c_type == LEX_ELSE) {
            gl();
            O();
        }
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);

    } else if (c_type == LEX_WHILE) {
        int pl0, pl1;
        gl();
        if (c_type != LEX_BRAC_OPEN) throw curr_lex;
        pl0 = prog.get_free();
        gl();
        E_as();
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type != LEX_BRAC_CLOSE) throw curr_lex;
        gl();
        start_cycle();
        st_break.push(0);
        O();
        finish_cycle();
        prog.put_lex(Lex(POLIZ_LABEL, pl0));
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        while (st_break.top()) {
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), st_break.top());
            st_break.pop();
        }
        st_break.pop();
    } else if (c_type == LEX_FOR) {
        int pl0, pl1, pl2, pl3;
        gl();
        if (c_type != LEX_BRAC_OPEN) throw curr_lex;
        gl();
        E_as();
        //prog.put_lex(Lex(POLIZ_DELETE));
        //check_id_in_for();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
        pl0 = prog.get_free();
        E_as();
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        //prog.put_lex(Lex(POLIZ_DELETE));
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        pl3 = prog.get_free();
        E_as();
        //prog.put_lex(Lex(POLIZ_DELETE));
        prog.put_lex(Lex(POLIZ_LABEL, pl0));
        prog.put_lex(Lex(POLIZ_GO));
        //check_id_in_for();
        if (c_type != LEX_BRAC_CLOSE) throw curr_lex;
        gl();
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
        start_cycle();
        st_break.push(0);
        O();
        finish_cycle();
        prog.put_lex(Lex(POLIZ_LABEL, pl3));
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        while (st_break.top()) {
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), st_break.top());
            st_break.pop();
        }
        st_break.pop();
    } else if (c_type == LEX_GOTO) {
        //list of undeclared labels for forward references
        gl();
        if (c_type != LEX_ID) throw curr_lex;
        if (TID[c_val].get_assign()) {
            //already can use
            prog.put_lex(Lex(POLIZ_LABEL, TID[c_val].get_value()));
            prog.put_lex(Lex(POLIZ_GO));
        } else if (!TID[c_val].get_declare()) {
            TID[c_val].put_declare();
            TID[c_val].put_mark();
            //append to list of labels
            v_labels[top_v_labels] = prog.get_free();
            TID[c_val].put_value(top_v_labels++);
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO));
        }
        gl();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
    } else if (c_type == LEX_READ) {
        gl();
        if (c_type != LEX_BRAC_OPEN) throw curr_lex;
        gl();
        if (c_type != LEX_ID) throw curr_lex;
        check_id_in_read();
        prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
        gl();
        if (c_type != LEX_BRAC_CLOSE) throw curr_lex;
        gl();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
        prog.put_lex(Lex(LEX_READ));
    } else if (c_type == LEX_WRITE) {
        gl();
        if (c_type != LEX_BRAC_OPEN) throw curr_lex;
        do{
            gl();
            E_as();
            prog.put_lex(Lex(LEX_WRITE));
        } while (c_type == LEX_COMA);
        if (c_type != LEX_BRAC_CLOSE) throw curr_lex;
        gl();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
    } else if (c_type == LEX_FIG_OPEN) {
    //composed operator
        gl();
        Os();
        if (c_type != LEX_FIG_CLOSE) throw curr_lex;
        gl();
    } else if (c_type == LEX_BREAK) {
        if (!is_in_cycle()) throw "usage of break out of cycle!";
        st_break.push(prog.get_free());
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        gl();
    } else if (c_type == LEX_ID) {
        int temp1 = c_val;
        int tem_val = c_val;
        gl();
        if (c_type == LEX_COLON) {
            if (!TID[tem_val].get_declare()) {
                TID[tem_val].put_declare();
                TID[tem_val].put_assign();
                TID[tem_val].put_mark();
                TID[tem_val].put_value(prog.get_free());
                gl();
            } else if (TID[tem_val].is_var()) throw "unknown operator ':' for variable!";
              else if (!TID[tem_val].get_assign()) {
                TID[tem_val].put_assign();
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), v_labels[TID[tem_val].get_value()]);
                v_labels[TID[tem_val].get_value()] = 0;
                TID[tem_val].put_value(prog.get_free());
                gl();
            } else throw "label can't be used for different operators!";
        } else if (c_type == LEX_ASSIGN) {
            tem_val = c_val;
            c_val = temp1;
            check_id();
            prog.put_lex(Lex(POLIZ_ADDRESS, temp1));
            //exchange to num when calculated
            c_val = tem_val;
            st_lex.push(LEX_ASSIGN);
            gl();
            E_as();
            check_op();
            prog.put_lex(Lex(POLIZ_DELETE));
            if (c_type != LEX_SEMICOLON) throw curr_lex;
            gl();
        } else if (c_type != LEX_SEMICOLON){
            c_val = temp1;
            check_id();
            c_val = tem_val;
            st_lex.push(c_type);
            gl();
            E_as();
            check_op();
            if (c_type != LEX_SEMICOLON) throw "; expected";
            gl();
        } else {
            c_val = temp1;
            check_id();
            st_lex.pop();
            c_val = tem_val;
            gl();
        }
    } else if (c_type == LEX_SEMICOLON) gl();
}

void Parser::E_as() {
//ASSIGN
    E_or();
    while (c_type == LEX_ASSIGN) {
        st_lex.push(LEX_ASSIGN);
        gl();
        E_or();
        check_op();
        st_ass.push(prog.pop_lex());
    }
    while (!st_ass.empty()) {
        prog.put_lex(st_ass.top());
        st_ass.pop();
    }
}

void Parser::E_or() {
//OR
    E_and();
    while (c_type == LEX_OR) {
        st_lex.push(LEX_OR);
        gl();
        E_and();
        check_op();
    }

}

void Parser::E_and(){
//AND
    E_rel();
    while (c_type == LEX_AND) {
        st_lex.push(LEX_AND);
        gl();
        E_rel();
        check_op();
    }
}

void Parser::E_rel() {
    E_p_m();
    while (c_type >= LEX_EQ && c_type <= LEX_NOTEQ) {
        st_lex.push(c_type);
        gl();
        E_p_m();
        check_op();
    }
}

void Parser::E_p_m() {
    E_p_d();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        st_lex.push(c_type);
        gl();
        E_p_d();
        check_op();
    }
}

void Parser::E_p_d() {
    E();
    while (c_type == LEX_TIMES || c_type == LEX_DIVIDE) {
        st_lex.push(c_type);
        gl();
        E();
        check_op();
    }
}

void Parser::E() {
    if (c_type == LEX_BRAC_OPEN) {
        gl();
        E_as();
        if (c_type != LEX_BRAC_CLOSE) throw curr_lex;
        gl();
    } else if (c_type == LEX_NOT) {
        gl();
        E();
        check_not();
    } else if (c_type == LEX_NUM) {
        st_lex.push(LEX_TYPE_INT);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_ID) {
        Lex temp;
        temp = curr_lex;
        check_id();//добавляется в стек
        gl();
        if (c_type == LEX_ASSIGN) {
            prog.put_lex(Lex(POLIZ_ADDRESS, temp.get_val()));
        } else {
            prog.put_lex(temp);
        }
    } else if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
        st_lex.push(LEX_TYPE_BOOL);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_CONST_STR) {
        st_lex.push(LEX_TYPE_STRING);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        Lex temp;
        if (c_type == LEX_PLUS) {
            temp = Lex(LEX_UNAR_PLUS);
        } else {
            temp = Lex(LEX_UNAR_MINUS);
        }
        gl();
        E();
        prog.put_lex(temp);
    } else if ((c_type != LEX_SEMICOLON) && (c_type != LEX_BRAC_CLOSE)) throw curr_lex;
}
