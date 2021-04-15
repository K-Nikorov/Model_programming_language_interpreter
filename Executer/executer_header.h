#ifndef EXECUTER_HEADER_H_INCLUDED
#define EXECUTER_HEADER_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;
//#include "../Scanner/scanner_header.h"
#include "../Syntex_Analyser/polize_header.h"

class Executer {
    Lex pc_el;
public:
    void execute(Poliz & prog);
    void struct_assign(int to, int from);
};

#endif // EXECUTER_HEADER_H_INCLUDED
