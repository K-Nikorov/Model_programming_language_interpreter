#include "lex_analyser_header.h"
#include "../common.h"

extern Table_Ident TID;
extern Table_Const_Str TCS;

int Lex::get_type() {return t_lex;}
int Lex::get_val() {return v_lex;}
ostream& operator<<(ostream& out, const Lex & l) {
    /*if (l.t_lex == LEX_ID) {
        out << "(" << "ID" << ", " << l.v_lex << ");";
    } else {
        int k = l.t_lex;
        for (int i = 1; dlms0[i]; ++i) {
            if (dlms0[i] == k) {k = i; break;}
        }
        if (dlms0[k])
            out << "(" << TD0[k] << ", " << l.v_lex << ");";
        else {
            for (int i = 1; words0[i]; ++i) {
                if (words0[i] == k) {k = i; break;}
            }
            out << "(" << TW0[k] << ", " << l.v_lex << ");";
        }
    }*/
    out << "(" << l.t_lex << ", " << l.v_lex << ");";
    return out;
}


Ident::Ident(const char* n) {
    name = new char[strlen(n)+1];
    strcpy(name, n);
}
Ident::~Ident() {delete[] name;}
Ident& Ident::operator=(Ident id) {
    this->put_name(id.get_name());
    var = id.get_var();
    struc = id.get_struc();
    label = id.get_label();
    declare = id.get_declare();
    type = id.get_type();
    assign = id.get_assign();
    value = id.get_value();
    pos_in_name = name + (id.get_pos() - id.get_name());
    return *this;
}
bool Ident::get_var() {return var;}
bool Ident::get_struc() {return struc;}
bool Ident::get_label() {return label;}
char* Ident::get_pos() {return pos_in_name;}
char* Ident::get_name() {return name;}
void Ident::put_name(const char* n) {
    if (!name) {
        name = new char[strlen(n)+6];
        strcpy(name, n);
    }
}
void Ident::add(int k) {
    if (k == 0) {
        *pos_in_name = '0';
        ++pos_in_name;
        return;
    }
    add(k / 10);
    *pos_in_name = '0' + k % 10;
    ++pos_in_name;

}
void Ident::remark_name(int k) {
    char c;
    int i = 0;
    do {
        c = name[i++];
    } while (c != ':');
    pos_in_name = name+i;
    add(k);
    *pos_in_name = '\0';
    pos_in_name = name;
}
bool Ident::get_declare() {return declare;}
void Ident::put_declare() {declare = true;}
int Ident::get_type() {return type;}
void Ident::put_mark() {var = false;}
bool Ident::is_var() {return var;}
void Ident::put_type(int t) {type = t;}
bool Ident::get_assign() {return assign;}
void Ident::put_assign() {assign = true;}
int Ident::get_value() {
    if (!struc) {
            return value;
    } else throw "struct type has not a value!";
}
void Ident::put_value(int v) {if (!struc) value = v; else throw "struct type has not a value!";}


Table_Const_Str::Table_Const_Str(int max_s) {
    p = new string[size=max_s];
    top = 1;
}
Table_Const_Str::~Table_Const_Str() {delete[] p;}
string& Table_Const_Str::operator[](int k) { return p[k];}
int Table_Const_Str::put(const char* buf) {
    /*for (int j=1; j<top; ++j)
        if (p[j] == buf) {return j;}*/
    p[top] = buf;
    ++top;
    return top-1;
}
void Table_Const_Str::print() {
    for (int i=0; i < top; ++i) {
        cout << p[i] << endl;
    }
}


Table_Ident::Table_Ident(int max_s) {
    p = new Ident[size=max_s];
    top = 1;
}
Table_Ident::~Table_Ident() { delete[] p;}
Ident& Table_Ident::operator[](int k) {return p[k];}
void Table_Ident::set_size(int s) {
    p = new Ident[size=s];
    top = 1;
}
int Table_Ident::put(const char* buf) {
    for(int j=1; j<top; ++j)
        if(!strcmp(buf, p[j].get_name()))
            return j;
    p[top].put_name(buf);
    ++top;
    return top-1;
}
int Table_Ident::put(Ident id) {
    p[top++] = id;
    return top-1;
}
int Table_Ident::get_top() {return top;}


void Struct::add_field(Ident* f) {
    if (num_of_fields < 14)
        fields[num_of_fields++] = f;
    else
        throw "too many fields in struct!";
}
int Struct::get_next() {
    cur_field = (cur_field+1) % 15;
    return cur_field;
}
void Struct::set_first(){
    cur_field = 0;
}
int Struct::get_top(){ return num_of_fields; }
Ident* & Struct::operator[](int k) {
    return fields[k];
}
Struct& Struct::operator=(Struct s) {
    s.set_first();
    this -> set_first();
    for (int i = 0; i < s.get_top(); ++i) {
        this->operator[](s.cur_field) = s[s.cur_field];
        get_next(); s.get_next();
    }
    this->num_of_fields = s.get_top();
    return *this;
}


Table_Struct::Table_Struct(int max_s) {
    p = new Struct[size=max_s];
    top = 1;
}
Table_Struct::~Table_Struct() {delete[] p;}
Struct& Table_Struct::operator[](int k) {return p[k];}
void Table_Struct::set_size(int s) {
    p = new Struct[size=s];
    top = 1;
}
int Table_Struct::put(Struct s) {
    p[top++] = s;
    return top-1;
}
int Table_Struct::get_top() {return top;}
