#include "polize_header.h"
#include "../common.h"

extern Table_Ident TID;
extern Table_Const_Str TCS;

Poliz::Poliz(int max_size) {p=new Lex[size=max_size]; free=0;}
Poliz::~Poliz() {delete[] p;}
void Poliz::put_lex(Lex l) {p[free++]=l;}
void Poliz::put_lex(Lex l, int place) {
    if (place < 0 || place >= size) throw "Poliz: place is out of range";
    p[place] = l;
}
Lex Poliz::pop_lex() {
    if (free > 0) {
        return p[--free];
    } else throw "POLIZ: poliz is empty!";
}
void Poliz::blank() {++free; if (free >= size) throw "Poliz: out of range";}
int Poliz::get_free(){return free;}
Lex& Poliz::operator[](int k) {
    if (k < 0 || k >= size) throw "Poliz: place is out of range";
    if (k > free) throw "Poliz: indefinite element";
    return p[k];
}
void Poliz::print() {
    for (int i=0; i < free; ++i) cout << p[i];
}

