#ifndef LEX_ANALYSER_HEADER_H_INCLUDED
#define LEX_ANALYSER_HEADER_H_INCLUDED


#include <iostream>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;

enum type_of_lex {
    LEX_NULL,
    //WORDS 1
    LEX_NOT, LEX_AND, LEX_OR, LEX_BOOL, LEX_ELSE,
    LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, LEX_PROGRAM,
    LEX_READ, LEX_TRUE, LEX_GOTO, LEX_FOR, LEX_WHILE, LEX_BREAK, LEX_WRITE,
    LEX_JUMP, LEX_FALSE_JUMP, LEX_THEN, LEX_DO,
    //DEVIDERS 24
    LEX_SEMICOLON, LEX_FIN, LEX_DOT, LEX_COMA, LEX_COLON, LEX_ASSIGN,
    LEX_BRAC_OPEN, LEX_BRAC_CLOSE,
    LEX_EQ, LEX_LSS, LEX_GRT, LEX_LSEQ, LEX_GTEQ, LEX_NOTEQ,
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_DIVIDE, LEX_FIG_OPEN, LEX_FIG_CLOSE,
    LEX_UNAR_PLUS, LEX_UNAR_MINUS, LEX_CONCAT, LEX_STRING_LSS, LEX_STRING_LSEQ,
    LEX_STRING_GRT, LEX_STRING_GTEQ, LEX_STRING_EQ, LEX_STRING_NOTEQ,
    LEX_STRING_ASSIGN, LEX_STRUCT_ASSIGN,
    LEX_NUM,//46
    LEX_CONST_STR,//47
    LEX_ID, //48
    LEX_STRUCT_FIELD,
    LEX_TYPE_INT, LEX_TYPE_BOOL, LEX_TYPE_STRING, LEX_TYPE_STRUCT,

    LEX_LAST_IN_LIST,

    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO, //41 !! they all here!
    POLIZ_DELETE
};

class Lex {
    int t_lex;
    int v_lex;
public:
    Lex(int t = LEX_NULL, int v = 0): t_lex(t), v_lex(v) {}
    int get_type(void);
    int get_val(void);
    friend ostream& operator<<(ostream& out, const Lex & l);
};

class Ident {
    char *name;
    bool var;
    bool struc;
    bool label;
    bool declare;
    int type;
    bool assign;
    int value;
    char* pos_in_name;

    void add(int k);
public:
    Ident(bool v=true, bool s=false): name(nullptr), var(v), struc(s), label(false), declare(false), assign(false), pos_in_name(nullptr) {}
    Ident(const char* n);
    ~Ident();
    Ident& operator=(Ident id);
    bool get_var();
    bool get_struc();
    bool get_label();
    char* get_pos();
    char *get_name();
    void put_name(const char* n);
    void remark_name(int k);
    bool get_declare();
    void put_declare();
    int get_type();
    void put_mark();
    bool is_var();
    void put_type(int t);
    bool get_assign();
    void put_assign();
    int get_value();
    void put_value(int v);
};

class Table_Const_Str {
    string* p;
    int size;
    int top;
public:
    Table_Const_Str(int max_s);
    ~Table_Const_Str();
    string& operator[](int k);
    int put(const char* buf);
    void print();
};

class Table_Ident{
    Ident* p;
    int size;
    int top;
public:
    Table_Ident(): size(0), top(0) {}
    Table_Ident(int max_s);
    ~Table_Ident();
    Ident& operator[](int k);
    void set_size(int s);
    int put(const char* buf);
    int put(Ident id);
    int get_top();
};

class Struct {
    Ident* fields[15];
    int num_of_fields;
public:
    int cur_field;
    Struct(): num_of_fields(0), cur_field(0) {}
    void add_field(Ident* f);
    int get_next();
    void set_first();
    int get_top();
    Ident* & operator[](int k);
    Struct& operator=(Struct s);
};

class Table_Struct {
    Struct* p;
    int size;
    int top;
public:
    Table_Struct(): size(0), top(0) {}
    Table_Struct(int max_s);
    ~Table_Struct();
    Struct& operator[](int k);
    void set_size(int s);
    //int put(const char* buf);
    int put(Struct s);
    int get_top();
};
#endif // LEX_ANALYSER_HEADER_H_INCLUDED
