#ifndef POLIZE_HEADER_H_INCLUDED
#define POLIZE_HEADER_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;

#include "../Scanner/scanner_header.h"


class Poliz {
    Lex *p;
    int size;
    int free;
public:
    Poliz(int max_size);
    ~Poliz() ;
    void put_lex(Lex l);
    void put_lex(Lex l, int place);
    Lex pop_lex();
    void blank();
    int get_free();
    Lex& operator[](int k);
    void print();
};


#endif // POLIZE_HEADER_H_INCLUDED
