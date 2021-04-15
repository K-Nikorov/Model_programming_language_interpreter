#ifndef INTERPRETOR_HEADER_H_INCLUDED
#define INTERPRETOR_HEADER_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;

#include "../Syntex_Analyser/parser_header.h"
#include "../Executer/executer_header.h"

class Interpretor {
    Parser pars;
    Executer E;
    char* prog;
public:
    void interpretation(char* prog);
};


#endif // INTERPRETOR_HEADER_H_INCLUDED
