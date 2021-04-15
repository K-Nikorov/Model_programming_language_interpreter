#include "main_header.h"
#include "common.h"

extern Table_Ident TID;
extern Table_Const_Str TCS;
extern Table_Struct TS;

int main()
{
    Parser pars;
    char prog_char[150];
    char c;
    int cur_ind_progname = 0;
    c = getchar();
    prog_char[0] = c;
    ++cur_ind_progname;
    while (c != '\n') {
        c = getchar();
        if (c == '\n') {
            prog_char[cur_ind_progname++] = '\0';
        } else {
            prog_char[cur_ind_progname++] = c;
        }
    }

    cout << prog_char << ": ";
    try{
        Interpretor I;
        I.interpretation(prog_char);

    }
    catch(char c) {
        cout << "\nnot a valid symbol: " << c << endl;
    }
    catch(const char* str) {
        cout << "\nerror: " << str << endl;
    }
    catch (Lex c_lex) {
        cout <<"\nmore complex problem: " << c_lex << endl;
    }
    catch (string str) {
        cout << "\nerror: " << str << endl;
    }

    return 0;
    //}
}
