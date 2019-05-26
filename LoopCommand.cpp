#include "LoopCommand.h"

/**
 * constructor
 * @param defineVarCommand the ref to the current define var command for the map op vars
 * @param commands the map of the commands
 */
LoopCommand::LoopCommand(DefineVarCommand *defineVarCommand, map<string, Command *> commands) {
    this->dfc = defineVarCommand;
    this->commandsMap = std::move(commands);
    commandsMap.insert(pair<string,Command*> ("while", this));
    commandsMap.insert(pair<string,Command*> ("if", new IfCommand(dfc,commandsMap)));
    commandsMap.insert(pair<string,Command*> ("print", new PrintCommand(dfc,commandsMap)));
    commandsMap.insert(pair<string,Command*> ("sleep", new SleepCommand()));
}
    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
int LoopCommand::doCommand(const vector<string> &vector, int index) {
    this->mapOfVars = this->dfc->getMap();
    int curIndex = index + 1;
    //create the vector of the conditions
    std::vector<string> conditions;
    while (vector[curIndex] != "{" || vector[curIndex] != "$") {
        conditions.push_back(vector[curIndex]);
        curIndex++;
        if (vector[curIndex] == "{" || vector[curIndex] == "$")
            break;
    }
    string op;
    std::vector<string> beforeTheOp;
    std::vector<string> afterTheOp;
    int condVsize = conditions.size();
    int i = 0;
    //until the value in the vector is not operator
    while (!isOperator(conditions[i])) {
        beforeTheOp.push_back(conditions[i]);
        i++;
    }
    //if its operator save it in the op string
    if (isOperator(conditions[i])) {
        op += conditions[i];
        //if its <= or >= or == or !=
        if (isOperator(conditions[i + 1])) {
            op += conditions[i + 1];
            i++;
        }
        i++;
    }
    //after the op is the left side of the cond
    while (i < condVsize) {
        afterTheOp.push_back(conditions[i]);
        i++;
    }
    //make the commands vector
    std::vector<string> commands;
    makeCommandsVector(vector,commands,curIndex);
    //do the commands in loop
    std::vector<string> lineOfCommands;
    recognizeLoopOp(commands,lineOfCommands, op, beforeTheOp, afterTheOp);
    return curIndex-index;
}
/**
 * make the vector of the comands
 * @param v the original vector
 * @param commands ref to the new vector of commands
 * @param curIndex the current index by ref
 */
void LoopCommand::makeCommandsVector(const vector<string> &v, vector<string> &commands, int & curIndex) {
    bool loopInLoop = false;
    //make the commands vector
    while (v[curIndex] != "}") {
        if ((v[curIndex]=="while") || (v[curIndex]=="if"))
            loopInLoop = true;
        commands.push_back(v[curIndex]);
        curIndex++;
    }
    //if there is loop in loop
    if (loopInLoop) {
        commands.push_back("}");
        commands.push_back("$");
        commands.push_back("End");
        curIndex += 2;
        makeCommandsVector(v,commands,curIndex);
    }
    if (!loopInLoop) {
        commands.push_back("}");
        commands.push_back("$");
        commands.push_back("EndWhile");
        curIndex += 2;
    }
}
/**
 * recognize the while loop
 * @param commands vector of the commands
 * @param lineCommands ref  to line of commands
 * @param op the current while operator
 * @param before the right side of the op
 * @param after the left side of the op
 */
void LoopCommand::recognizeLoopOp(const vector<string> &commands, vector<string> &lineCommands,string op,
        const vector<string> &before, const vector<string> &after) {
    EqualsCommand* equalsCommand = (EqualsCommand*)this->commandsMap["="];
    vector<string> temp;
    //evaluate every side in the conditions
    double a = evaluateTheString(before, equalsCommand, temp);
    double b = evaluateTheString(after, equalsCommand, temp);
    if (op == "<") {
        while (a < b) {
            doWhileCommand(commands, lineCommands);
            a = evaluateTheString(before, equalsCommand, temp);
            b = evaluateTheString(after, equalsCommand, temp);
        }
    }
    if (op == ">") {
        while (a > b) {
            doWhileCommand(commands, lineCommands);
            a = evaluateTheString(before, equalsCommand, temp);
            b = evaluateTheString(after, equalsCommand, temp);
        }
    }
    if (op == "<=") {
        while (a <= b) {
            doWhileCommand(commands, lineCommands);
            a = evaluateTheString(before, equalsCommand, temp);
            b = evaluateTheString(after, equalsCommand, temp);
        }
    }
    if (op == ">=") {
        while (a >= b) {
            doWhileCommand(commands, lineCommands);
            a = evaluateTheString(before, equalsCommand, temp);
            b = evaluateTheString(after, equalsCommand, temp);
        }
    }
    if (op == "!=") {
        while (a != b) {
            doWhileCommand(commands, lineCommands);
            a = evaluateTheString(before, equalsCommand, temp);
            b = evaluateTheString(after, equalsCommand, temp);
        }
    }
    if (op == "==") {
        while (a == b) {
            doWhileCommand(commands, lineCommands);
            a = evaluateTheString(before, equalsCommand, temp);
            b = evaluateTheString(after, equalsCommand, temp);
        }
    }
}

/**
 * do the while command. seperate the lines in the commands vector and execute each of them.
 * @param commands vector of the commands
 * @param lineCommands ref to line vector
 */
void LoopCommand::doWhileCommand(const vector<string> &commands, vector<string> &lineCommands) {
    int i = 0;
    bool ifOrWhile = true;
    while (commands[i] != "EndWhile") {
        while (commands[i] != "$") {
            lineCommands.push_back(commands[i]);
            i++;
        }
        if (commands[i] == "$") {
            i++;
        }
        //if its loop in loop
        if((lineCommands[0] == "while") || (lineCommands[0] == "if")) {
            lineCommands.push_back("$");
            if (lineCommands[0] == "while")
                ifOrWhile = false;
            while (commands[i] != "End") {
                lineCommands.push_back(commands[i]);
                i++;
            }
            if (commands[i] == "End") {
                if (ifOrWhile)
                    lineCommands.push_back("EndIf");
                else
                    lineCommands.push_back("EndWhile");
                i++;
            }
        }
        lineCommands.push_back("$");
        //now we have vector of line of commands.
        //send it to the do-command function of while.
        executeLoop(lineCommands);
        lineCommands.clear();
    }
}
/**
 * execute the line
 * @param lineCommands ref to the vector of line commands
 */
void LoopCommand::executeLoop(vector<string> &lineCommands) {
    int size = lineCommands.size();
    if (size > 1) {
        //if the line command end with }$
        if (lineCommands.at(size - 2) == "}") {
            lineCommands.pop_back();
            lineCommands.pop_back();
            lineCommands.push_back("$");
        }
    }
    int index = 0;
    while (index < size) {
        Command *c = commandsMap[lineCommands[index]];
        if (c != NULL) {
            index += c->doCommand(lineCommands, index);
            break;
        }
        index++;
    }
}
/**
 * check if the string is number
 * @param s string
 * @return true if its number anf false otherwise
 */
bool LoopCommand::is_number(const std::string &s){
    char *end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end!= s.c_str() && val != HUGE_VAL;
}
/**
 * check if the string is operator
 * @param s string
 * @return true if its operator and false otherwise
 */
bool LoopCommand::isOperator (string op) {
    return op == "<"||op == ">"||op == "="||op == "!";
}
/**
 * check if the string is expresion
 * @param s string
 * @return true if its expresion and false otherwise
 */
bool LoopCommand::isExp(string exp) {
    return exp == "+" ||exp == "-" ||exp == "*" ||exp == "/";
}

/**
 * evaluate the string in the vector
 * @param s left or righr condition vector
 * @param e ref to equal command
 * @param temp temp vector .
 * @return value of the calculating
 */
double LoopCommand::evaluateTheString (const vector<string> &s, EqualsCommand* e, vector<string> &temp) {
    map<string, pair<string, double>>::iterator it;
    //if its one parm its have to be a var or double
    if (s.size() == 1) {
        if (is_number(s[0])) {
            return atof(s[0].c_str());
        } else {
            it = mapOfVars->find(s.at(0));
            return it->second.second;
        }
    } else {  //if its more than one parm
        temp.clear();
        int size = s.size();
        //push each value to the temp vector
        for (int i = 0; i < size; i++) {
            string current = s[i];
            //if its a numer or operator push him
            if (is_number(current) || isOperator(current)) {
                temp.push_back(current);
            } else {  ///if its a var, find its value and push it to the vec
                it = mapOfVars->find(s.at(0));
                temp.push_back(to_string(it->second.second));
            }
        }
        //evaluate the vector and return the result
        return e->evaluate(temp);
    }
}