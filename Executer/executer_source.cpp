#include "executer_header.h"
#include "../common.h"

extern Table_Ident TID;
extern Table_Const_Str TCS;
extern Table_Struct TS;

void Executer::execute(Poliz & prog) {
    cout << "Executor!\n";
    stack <int> args;
    int i,j,index=0, size = prog.get_free();
    int last_type;
    while (index < size) {
       // cout << "{{ index = " << index << " }}" << endl;
        pc_el = prog[index];
        switch (pc_el.get_type()) {
        case LEX_TRUE: case LEX_FALSE:
            {
                i = (pc_el.get_type() == LEX_TRUE) ? 1 : 0;
                args.push(i);
                last_type = pc_el.get_type();
                break;
            }
        case LEX_NUM: case LEX_CONST_STR:
        case POLIZ_ADDRESS: case POLIZ_LABEL:
            {
                args.push(pc_el.get_val());
                last_type = pc_el.get_type();
                break;
            }
        case LEX_ID:
            {
                i = pc_el.get_val();
                if (TID[i].get_assign()) {
                    last_type = TID[i].get_type();
                    args.push(TID[i].get_value());
                    break;
                } else {
                    string mist = "POLIZ: indefinite identifier: ";
                    mist += TID[i].get_name();
                    throw mist;
                }
            }
        case LEX_NOT:
            {
                i = !args.top();
                args.pop();
                args.push(i);
                break;
            }
        case LEX_OR:
            {
                i = args.top();
                args.pop();
                j = args.top();
                args.pop();
                args.push(i || j);
                break;
            }
        case LEX_AND:
            {
                i = args.top();
                args.pop();
                j = args.top();
                args.pop();
                args.push(i && j);
                break;
            }
        case POLIZ_GO:
            {
                i = args.top();
                args.pop();
                index = i - 1;
                break;
            }
        case POLIZ_FGO:
            {
                i = args.top();
                args.pop();
                j = args.top();
                args.pop();
                if (!j) index = i - 1;
                break;
            }
        case POLIZ_DELETE:
            {
                args.pop();
                break;
            }
        case LEX_WRITE:
            {
                i = args.top(); args.pop();
                if (last_type == LEX_TYPE_INT) {
                    cout << i;
                } else if (last_type == LEX_TYPE_BOOL) {
                    if (!i)
                        cout << "false";
                    else
                        cout << "true";
                } else if (last_type == LEX_TRUE) {
                    cout << "true";
                } else if (last_type == LEX_FALSE) {
                    cout << "false";
                } else if (last_type == LEX_TYPE_STRING || last_type == LEX_CONST_STR) {
                    cout << TCS[i];
                }
                cout << endl;
                break;
            }
        case LEX_READ:
            {
                int in;
                i = args.top(); args.pop();
                if (TID[i].get_type() == LEX_TYPE_INT) {
                    cin >> in;
                    TID[i].put_value(in);
                } else if (TID[i].get_type() == LEX_TYPE_BOOL) {
                    string b;
                    cin >> b;
                    if (b == "true")
                        in = 1;
                    else if (b == "false")
                        in = 0;
                    else throw "try to read not boolean value in bool variable";
                    TID[i].put_value(in);
                } else if (TID[i].get_type() == LEX_TYPE_STRING) {
                    string st;
                    getline(cin, st);
                    if (st == "")
                        getline(cin, st);
                    TCS[TID[i].get_value()].clear();
                    TCS[TID[i].get_value()] += st;
                } else throw "inappropriate type for read operator";
                TID[i].put_assign();
                break;
            }
        case LEX_PLUS:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(i+j);
                break;
            }
        case LEX_UNAR_PLUS:
            {
                i = args.top(); args.pop();
                args.push(i);
                break;
            }
        case LEX_CONCAT:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS.put((TCS[j] + TCS[i]).c_str()));
                break;
            }
        case LEX_MINUS:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(j-i);
                break;
            }
        case LEX_UNAR_MINUS:
            {
                i = args.top(); args.pop();
                args.push(-i);
                break;
            }
        case LEX_TIMES:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(i * j);
                break;
            }
        case LEX_DIVIDE:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                if (!i) { args.push(j/i); break; }
                else throw "Poliz: division by zero";
            }
        case LEX_EQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(i == j);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_STRING_EQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS[j] == TCS[i]);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_NOTEQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(i != j);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_STRING_NOTEQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS[j] != TCS[i]);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_LSS:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(j < i);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_STRING_LSS:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS[j] < TCS[i]);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_GRT:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(j > i);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_STRING_GRT:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS[j] > TCS[i]);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_LSEQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(j <= i);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_STRING_LSEQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS[j] <= TCS[i]);
                break;
            }
        case LEX_GTEQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(j >= i);
                break;
            }
        case LEX_STRING_GTEQ:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                args.push(TCS[j] >= TCS[i]);
                last_type = LEX_TYPE_BOOL;
                break;
            }
        case LEX_ASSIGN:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                TID[j].put_value(i);
                TID[j].put_assign();
                args.push(i);
                //cout << "execute assign: " << j << ", " << TID[j].get_value() << endl;
                break;
            }
        case LEX_STRING_ASSIGN:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();
                if (!TID[j].get_assign()) {
                    TCS[TID[j].get_value()].clear();
                }
                if (last_type == LEX_TYPE_STRING) {
                    TCS[TID[j].get_value()] = TCS[i];
                    cout << TCS[TID[i].get_value()] << endl;
                } else if (last_type == LEX_CONST_STR) {
                    TCS[TID[j].get_value()] = TCS[i];
                }
                args.push(TID[j].get_value());
                TID[j].put_assign();
                break;
            }
        case LEX_STRUCT_ASSIGN:
            {
                i = args.top(); args.pop();
                j = args.top(); args.pop();

                struct_assign(j, i);
                args.push(TID[j].get_value());
                break;
            }
        default: throw "Poliz: unexpected element";
        }
        // after switch
        ++index;
    }
}

void Executer::struct_assign(int to, int from) {
    TS[TID[to].get_value()].set_first();
    TS[from].set_first();

    for (int j=0; j < TS[from].get_top(); ++j) {
        Ident* id = TS[from][j];
        /*cout << "Type: " << from << "--" << "Num: " << k << endl;
        cout << "Name: " << TID[k].get_name() << endl;*/
        if (id->get_type() == LEX_TYPE_STRING) {
            TCS[TS[TID[to].get_value()][j]->get_value()].clear();
            TCS[TS[TID[to].get_value()][j]->get_value()].append(TCS[id->get_value()]);
        } else if (id->get_type() > LEX_LAST_IN_LIST) {
            struct_assign(TID.put(TS[TID[to].get_value()][j]->get_name()), id->get_value());
        } else {
            TS[TID[to].get_value()][j]->put_value(id->get_value());
        }
    }
}
