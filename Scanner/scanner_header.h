#ifndef SCANNER_HEADER_H_INCLUDED
#define SCANNER_HEADER_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;

#include "../Lex Analyser/lex_analyser_header.h"

class Scanner {
    enum state {H, IDENT, NUMB, CONST_STR, COM, ALE, DELIM, NEQ, PROG_INFO};
    static const char* TW[];
    static const type_of_lex words[];
    static const char *TD[];
    static const type_of_lex dlms[];

    state cs;
    FILE* fp;
    char c;
    char buf[86];
    int buf_top;
    stack < int > st_inherit;

    void clear();
    void add();
    void add(int num);
    int look(const char* buf, const char** list) const;
    void gc();
public:
    Scanner() {}
    void scan_prog(const char* prog);
    void close_prog(const char* prog);
    Lex get_lex();
};



#endif // SCANNER_HEADER_H_INCLUDED
