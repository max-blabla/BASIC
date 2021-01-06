/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
   // Replace this stub with your own code

}

Program::~Program() {
    sourcelist.clear();
    statementlist.clear();
   // Replace this stub with your own code
}

void Program::clear() {
    sourcelist.clear();
    statementlist.clear();
   // Replace this stub with your own code
}

void Program::addSourceLine(int lineNumber, string line) {
    if(sourcelist.count(lineNumber)){
    sourcelist.erase(lineNumber);}
    sourcelist.insert(pair<int,string>(lineNumber,line));
   // Replace this stub with your own code
}

void Program::removeSourceLine(int lineNumber) {
    if(sourcelist.count(lineNumber))
        sourcelist.erase(lineNumber);
    if(statementlist.count(lineNumber))
        statementlist.erase(lineNumber);

   // Replace this stub with your own code
}

string Program::getSourceLine(int lineNumber) {
    map<int,string>::iterator iter;
    iter=sourcelist.find(lineNumber);
   return iter->second;
   // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if(statementlist.count(lineNumber))
        statementlist.erase(lineNumber);
    statementlist.insert(pair<int,Statement*>(lineNumber,stmt));
   // Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber) {
    return statementlist[lineNumber];  // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
   map<int,Statement*>::iterator code_iter;
   code_iter=statementlist.begin();
   return code_iter->first;    // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
    map<int,Statement*>::iterator code_iter;
    code_iter=statementlist.find(lineNumber);
    code_iter++;
    if(code_iter!=statementlist.end())
        return code_iter->first;
    return 0;// Replace this stub with your own code
}

bool Program::empty(){
    if(sourcelist.empty())
        return 1;
    return 0;
}
bool Program::find(int linenumber){
    return sourcelist.count(linenumber);
}