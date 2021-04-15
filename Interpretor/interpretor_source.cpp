#include "interpretor_header.h"
#include "../common.h"

extern Table_Const_Str TCS;
extern Table_Ident TID;
extern Table_Struct TS;

void Interpretor::interpretation(char* prog) {
    pars.analyze(prog);
    std::cout << "\n\n\n\n";
    E.execute(pars.prog);
}
