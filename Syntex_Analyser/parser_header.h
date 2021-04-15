#ifndef PARSER_HEADER_H_INCLUDED
#define PARSER_HEADER_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;
//#include "../Scanner/scanner_header.h"
#include "polize_header.h"


class Parser {
    Lex curr_lex;
    int c_type;
    int c_val;
    int in_cycle;
    int in_struc;
    int top_v_labels;
    int last_struc;
    bool descr_permit;
    bool struc_permit;
    Scanner scan;

    stack <int> st_struc;
    stack <Ident*> st_fields;
    stack <int> st_lex;
    stack <int> st_break;
    stack <Lex> st_ass;
    vector <int> v_labels;

    void P(); void Ds(bool); void D(bool); void S_Ds(); void T();
    void V_int(bool); void V_bool(bool); void V_str(bool); void V_struc(bool); void V_comp_type(bool);
    void C(); void N(); void Str();
    void Sgn(); void Os(); void E_as(); void E_or();
    void E_and(); void E_p_d(); void E_p_m();
    void E_rel(); void E(); void O(); void CO();
    void OE();

    void check_id(); void check_op(); void check_not();
    void eq_bool(); void check_id_in_read(); void check_id_in_for();

    void start_cycle();
    void finish_cycle();
    bool is_in_cycle();

    void forbid_descr();
    void forbid_struc();
    void copy_struct(int, int);
    void get_in_struc();
    void out_of_struc();
    bool is_in_struc();
    void gl();

public:
    Poliz prog;
    Parser(): in_cycle(0), top_v_labels(0), descr_permit(true), struc_permit(true), v_labels(100, 0), prog(1000) {}
    void parse_prog(const char* program);
    void close_prog(const char* program);
    void analyze(const char* program);
};


#endif // PARSER_HEADER_H_INCLUDED
