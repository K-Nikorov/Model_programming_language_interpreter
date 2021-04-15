#include "scanner_header.h"
#include "../common.h"

extern Table_Ident TID;
extern Table_Const_Str TCS;

const char* Scanner::TW[] = {NULL, "and", "bool", "do", "else",
                        "if", "false", "int", "string", "struct", "not", "or", "program", "read",
                        "then", "true", "goto", "for", "while", "break", "write", "!", "!F", NULL};
const type_of_lex Scanner::words[] = {LEX_NULL, LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE,
                            LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, LEX_NOT, LEX_OR, LEX_PROGRAM,
                            LEX_READ, LEX_THEN, LEX_TRUE, LEX_GOTO, LEX_FOR, LEX_WHILE, LEX_BREAK, LEX_WRITE,
                            LEX_JUMP, LEX_FALSE_JUMP, LEX_NULL};

const char* Scanner::TD[] = {NULL, ";", "@", ".", ",", ":", "=", "(", ")", "==", "<", ">",
                        "+","-","*","/","<=","!=",">=", "{", "}", NULL};
const type_of_lex Scanner::dlms[] = {LEX_NULL, LEX_SEMICOLON, LEX_FIN, LEX_DOT, LEX_COMA, LEX_COLON, LEX_ASSIGN,
                                LEX_BRAC_OPEN, LEX_BRAC_CLOSE, LEX_EQ, LEX_LSS, LEX_GRT,
                                LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_DIVIDE, LEX_LSEQ,
                                LEX_NOTEQ, LEX_GTEQ, LEX_FIG_OPEN, LEX_FIG_CLOSE, LEX_NULL};

void Scanner::clear() {
    for (int j=0; j < 86; ++j)
        buf[j] = '\0';
    buf_top = 0;
}
void Scanner::add() {
    if (buf_top < 79)
        buf[buf_top++] = c;
    else {
        cout << "warning: too long identifier name: " << buf << endl;
    }
}
void Scanner::add(int num){
    if (num == 0) {
        buf[buf_top++] = '0';
        return;
    }
    add(num / 10);
    buf[buf_top++] = '0' + num % 10;

}
int Scanner::look(const char* buf, const char** list) const {
    int i = 1;
    //possible mistake!
    while (list[i]){
        if(!strcmp(buf, list[i]))
            return i;
        ++i;
    }
    return 0;
}
void Scanner::gc() {c = fgetc(fp);}
void Scanner::scan_prog(const char* prog) {
    fp = fopen(prog, "r");
    cs = H;
    clear();
    gc();
}
void Scanner::close_prog(const char* prog) {
    fclose(fp);
}
Lex Scanner::get_lex(){
    int d,j;
    cs = H;
    do {
        switch (cs) {
    case H:
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                gc();
            } else if (isalpha(c)) {
                j = 0;
                clear();
                add();
                gc();
                cs = IDENT;
            } else if (isdigit(c)) {
                d = c - '0';
                gc();
                cs = NUMB;
            } else if (c == '.') {
                return Lex(LEX_DOT);
            } else if (c == '#') {
                gc();
                cs = COM;
            } else if (c == '"') {
                clear();
                gc();
                cs = CONST_STR;
            } else if (c == '=' || c == '<' || c == '>') {
                clear();
                add();
                gc();
                cs = ALE;
            } else if (c == '@') {
                return Lex(LEX_FIN);
            } else if (c == '!') {
                clear();
                add();
                gc();
                cs = NEQ;
            } else if (c == '$') {
                TCS.print();
                gc();
            } else if (c == '^') {
                gc();
                cs = PROG_INFO;
            } else {
                cs = DELIM;
            }
            break;
    case IDENT:
            if(isalpha(c) || isdigit(c)) {
                add();
                gc();
            } else if((j = look(buf, TW)) != 0) {
                return Lex(words[j], j);
            } else if (c == '.') {
                if (!st_inherit.empty()) {
                    j = st_inherit.top();
                    st_inherit.pop();
                }
                c = ':'; add();
                add(j);
                j = TID.put(buf);
                st_inherit.push(j);
                if (!TID[j].get_declare()) throw "undefined name of struct!";
                if (TID[j].get_type() < LEX_LAST_IN_LIST) throw "using non-struc object as struc!";
                clear();
                gc();
            } else {
                char t = c;
                if (!st_inherit.empty()) {
                    j = st_inherit.top();
                    st_inherit.pop();
                }
                c = ':'; add();
                add(j);
                c = t;
                j = TID.put(buf);
                return Lex(LEX_ID, j);
            }
            break;
    case NUMB:
            if (isdigit(c)) {
                d = d*10 + c-'0';
                gc();
            } else {
                return Lex(LEX_NUM, d);
            }
            break;
    case CONST_STR:
            while (c != '"') {
                if (c == '@') throw c;
                add();
                gc();
            }
            gc();
            j = TCS.put(buf);
            return Lex(LEX_CONST_STR, j);
            break;
    case COM:
            if (c == '#') {
                gc();
                cs = H;
            } else if (c == '@') {
                throw c;
            } else {
                gc();
            }
            break;
    case ALE:
            if (c == '=') {
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j], j);
            } else {
                //gc();
                j = look(buf, TD);
                return Lex(dlms[j], j);
            }
            break;
    case NEQ:
            if (c == '=') {
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j], j);
            } else {
                throw '!';
            }
            break;
    case DELIM:
            clear();
            add();
            j = look(buf, TD);
            if (j) {
                gc();
                return Lex(dlms[j], j);
            } else {
                throw c;
            }
            break;
    case PROG_INFO:
            do {
                cout << c;
                gc();
            } while (c != '^');
            cout << "\n\n";
            gc();
            cs = H;
            break;
        }
    } while(true);
}
