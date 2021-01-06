/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */
bool check(string statement);
void processLine(string line, Program & program, EvalState & state);
void runit(string statement, const char* str,Program & program, EvalState & state);
void parsestatement(string line, Program & program, EvalState & state);
/* Main program */

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * needre to place this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
    int position=0;
    while(true){
        if(line[position]!=' ')
            break;
        position++;
    }
    if(position==strlen(line.c_str())){
        return;
    }
    parsestatement(line.c_str()+position,program,state);
}
void parsestatement(string line, Program & program, EvalState & state){
    //立即执行的
    if(line[0]<'0'||line[0]>'9'){
        int position=0;
        string statement="";
        while(true){
            if(line[position]==' '||line[position]=='\0')
                break;
            statement+=line[position];
            position++;
        }
        //加一个认不出是什么玩意的报错
        bool flag=check(statement);
        if(flag)
            runit(statement,line.c_str()+position,program,state);
        else{
            Statement* stmt;
            stmt=stmt->parse(line.c_str());
            stmt->execute(state);
        }
    }
    //非即使执行的
    else{
        int linenumber=0;
        int position=0;//下标位置
        while(true){
            linenumber=linenumber*10+line[position]-'0';
            position++;
            if(line[position]==' '||line[position]=='\0')
                break;
        }
        //是消除吗？
        if(position==strlen(line.c_str())){
            program.removeSourceLine(linenumber);
            return;
        }
        while(true){
            //跳过奇怪的空格
            position++;
            if(line[position]==' '||line[position]=='\0')
                continue;
            break;
        }
        position--;
        program.addSourceLine(linenumber,line.c_str()+position);
        Statement* stmt;
        stmt=stmt->parse(line.c_str()+position);
        program.setParsedStatement(linenumber,stmt);
    }
}
bool check(string statement){
    if(statement=="RUN"||statement=="LIST"||statement=="CLEAR"||statement=="QUIT"||statement=="HELP"||statement=="INPUT"||statement=="PRINT"||statement=="LET")
        return 1;
    else if(statement=="GOTO"||statement=="IF"||statement=="REM"||statement=="END")
        return 0;
    error("SYNTAX ERROR");
}
void runit(string statement,const char* str, Program & program, EvalState & state){
    if(statement=="RUN"){
        try{int position=program.getFirstLineNumber();
        do{
            //加入时应该不能解析，不然会出错(其实不会，因为实例化在eval中完成）
            Statement* tmp;
            tmp=program.getParsedStatement(position);
            tmp->execute(state);
            //加上读写和理解BASIC语句
            int tmpn=tmp->getgotoline();
            if(tmpn==-1)
            position=program.getNextLineNumber(position);
            else if(!program.find(tmpn))
                throw LINENUMBERERROR();
            else
                position=tmpn;
            } while(position);
        }catch(END){return;}
         catch(LINENUMBERERROR){error("LINE NUMBER ERROR");}
    }
    if(statement=="LIST"){
        int position=program.getFirstLineNumber();
        do{
            string line=program.getSourceLine(position);
            //输出那些东西即可
            if(!program.empty()){
            cout<<position<<line<<endl;
            position=program.getNextLineNumber(position);}
        } while(position);

    }
    if(statement=="PRINT"){
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(str);
        Expression* exp=parseExp(scanner);
        int value=exp->eval(state);
        cout<<value<<endl;
        delete exp;
    }
    if(statement=="LET"){
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(str);
        Expression* exp=parseExp(scanner);
        exp->eval(state);
        delete exp;
    }
    if(statement=="INPUT"){
        int value=0;
        string tmp;
        int position=0;
        while(true){
            if(str[position]==' '){
                position++;
                continue;}
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
          }catch(INVALIDNUMBER){error("INVALID NUMBER");};
        }
        int length=strlen(tmp.c_str());
        for(int i=0;i<length;i++)
            value=value*10+tmp[i]-'0';
        state.setValue(str+position,value);
    }
    if(statement=="CLEAR"){
        program.clear();
        state.clear();
    }
    if(statement=="QUIT"){
        //要记得删东西
        program.clear();
        state.clear();
        exit(0);
    }
    if(statement=="HELP"){
        cout<<"Yet another basic interpreter"<<endl;
    }
}
