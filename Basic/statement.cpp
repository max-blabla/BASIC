/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include"string.h"
#include "statement.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "parser.h"
#include "program.h"
using namespace std;

/* Implementation of the Statement class */


Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}
Statement* Statement::parse(const char* statementsourceline) {
    int position=0;
    string statement="";
    while(true){
        position++;
        if(statementsourceline[position]==' '||statementsourceline[position]=='\0')
            break;
        statement+=statementsourceline[position];
    }
    if(statement=="REM"||statement=="LET"||statement=="PRINT"||statement=="INPUT"||statement=="END")
        return new SequentialStatement(statement,statementsourceline+position);
    if(statement=="GOTO"||statement=="IF")
        return new ControlStatement(statement,statementsourceline+position);
    error("SYNTAX ERROR");
}
int SequentialStatement::getgotoline() {
    return gotoline;
}
SequentialStatement::SequentialStatement(string statement,const char* statementsourceline){
    op=statement;
    sourceline=statementsourceline;
    if(statement=="LET"||statement=="PRINT"){
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(sourceline);
        parsedexp=parseExp(scanner);
    }
};
void SequentialStatement::execute(EvalState&state){
    if(op=="REM"){}
    if(op=="LET") {int value = parsedexp->eval(state);}
    if(op=="PRINT"){//这里要注意error之类的
        int value = parsedexp->eval(state);
        cout<<value<<endl; }
    if(op=="INPUT"){
        int value=0;
        string tmp;
        int position=0;
        while(true){
            if(sourceline[position]==' '){
                position++;
                continue;
            }
            break;
        }
        while(true){
            try{
                cout<<" ? ";
                getline(cin,tmp);
                int length=strlen(tmp.c_str());
                for(int i=0;i<length;i++){
                    if(tmp[i]>'9'||tmp[i]<'0')
                        throw(INVALIDNUMBER());
                }
                break;
            }catch(INVALIDNUMBER){cout<<"INVALID NUMBER"<<endl;};
        }
        int length=strlen(tmp.c_str());
        for(int i=0;i<length;i++)
            value=value*10+tmp[i]-'0';
        state.setValue(sourceline.c_str()+position,value);
    }
    if(op=="END"){throw END();}
}
;
SequentialStatement::~SequentialStatement(){
    delete parsedexp;
};
ControlStatement::ControlStatement(string statement,const char* statementsourceline){
    op=statement;
    if(op=="IF"){
        int position=0;
        string ifline="";
        while(true){
            position++;
            if(statementsourceline[position]=='T'&&statementsourceline[position+1]=='H'&&statementsourceline[position+2]=='E'&&statementsourceline[position+3]=='N'){
                for(int i=0;i<position;i++)
                    ifline+=statementsourceline[i];
                position+=4;
                while(statementsourceline[position]!='\0'){
                    sourceline+=statementsourceline[position];//sourceline记then后的
                    position++;
                }
                break;
            }//加逐句分析
        }
        if(position==strlen(ifline.c_str()))
            error("SYNTAX ERROR");
        //对IF后分析
        position=0;
        string left="";
        string right="";
        string token;
        while(true){
            position++;
            if(ifline[position]=='<'||ifline[position]=='>'||ifline[position]=='='){
                token=ifline[position];
                for(int i=0;i<position;i++)
                    left+=ifline[i];
                for(int i=position+1;ifline[i]!='\0';i++)
                    right+=ifline[i];
                TokenScanner scanner;
                scanner.ignoreWhitespace();
                scanner.scanNumbers();
                scanner.setInput(left);
                judgeleft=parseExp(scanner);
                TokenScanner scanner1;
                scanner1.ignoreWhitespace();
                scanner1.scanNumbers();
                scanner1.setInput(right);
                judgeright=parseExp(scanner1);
                break;
            }
        }
        //对THEN后解析
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(sourceline);
        parsedexp=parseExp(scanner);
        //组合成新的
        if(token=="=")
            token="==";
        judgeline=new CompoundExp(token,judgeleft,judgeright);
    }
    if(op=="GOTO"){
        sourceline=statementsourceline;
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(sourceline);
        parsedexp=parseExp(scanner);
    }
}
int ControlStatement::getgotoline() {
    return gotoline;
}
void ControlStatement::execute(EvalState &state) {
    if(op=="GOTO")
        gotoline=parsedexp->eval(state);
    if(op=="IF"){
        if(judgeline->eval(state))
            gotoline=parsedexp->eval(state);
        else
            gotoline=-1;
    }
}
ControlStatement::~ControlStatement(){
    delete parsedexp;
    delete judgeline;
    delete judgeleft;
    delete judgeright;
}