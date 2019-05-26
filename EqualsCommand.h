#ifndef PROJECT_EQUALSCOMMAND_H
#define PROJECT_EQUALSCOMMAND_H


#include <map>
#include "Command.h"
#include "DefineVarCommand.h"
#include "Expression.h"
#include "ConnectToServer.h"
#include "ConnectCommand.h"
#include <thread>
#include "DefineVarCommand.h"
#include <map>
#include <iostream>
#include "Expression.h"
#include "Number.h"
#include "Mul.h"
#include "Minus.h"
#include "Plus.h"
#include "Divi.h"
#include "ConnectToServer.h"
#include "ConnectCommand.h"
#include <bits/stdc++.h>
#include <stack>


class EqualsCommand : public Command{
    map<string, pair<string, double>> *mapOfVars;
    DefineVarCommand* dfc;
    ConnectCommand *cnctCmd;
    ConnectToServer* connectToServer;
    map<string, Command*> commandMap;

public:
    EqualsCommand(DefineVarCommand *varCommand, map<string, Command*> commands);
    int doCommand(const vector<string> &vector, int index) override;
    double evaluate(const vector<string> &old_vector);
    Expression* applyOp(Expression* a, Expression* b, string op);
    int precedence(string op);
    void zeroMinus (const vector<string> &old_vector, vector<string> &tokens);
    bool is_number(const string &s);
    bool isExp (const string &exp);
    ~EqualsCommand() override {};
};


#endif //EVEN1_EQUALSCOMMAND_H
