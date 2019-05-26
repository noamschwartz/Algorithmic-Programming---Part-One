
#include "IfCommand.h"

/**
 * constructor
 * @param defineVarCommand the ref to the current define var command for the map op vars
 * @param commands the map of the commands
 */
IfCommand::IfCommand(DefineVarCommand *defineVarCommand, map<string, Command *> commands) {
    this->commandsMap = std::move(commands);
    this->dfc = defineVarCommand;
    commandsMap.insert(pair<string,Command*> ("if", this));
    commandsMap.insert(pair<string,Command*> ("print", new PrintCommand(dfc,commandsMap)));
    commandsMap.insert(pair<string,Command*> ("sleep", new SleepCommand()));
}
    /**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
int IfCommand::doCommand(const vector<string> &vector, int index) {
    this->mapOfVars = this->dfc->getMap();
    map<string, pair<string, double>>::iterator it;
    int curIndex = index + 1;
    std::vector<string> conditions;
    while (vector[curIndex] != "{" || vector[curIndex] != "$") {
        conditions.push_back(vector[curIndex]);
        curIndex++;
        if (vector[curIndex] == "{" || vector[curIndex] == "$")
            break;
    }
    int condVsize = conditions.size();
    double firstC, secondC;
    string op;
    //the conditions vector have to be 4 or 3 items.
    //if the op is "<=" ">=" "==" "!=" the size will be 4 otherwise its will be 3.
    if (condVsize > 4) {
        EqualsCommand* e = (EqualsCommand*)this->commandsMap["="];
        std::vector<string> temp_vector;
        //its have to be an expression.
        int i = 0;
        int j = 0;
        while (!isOperator(conditions.at(i)))
            i++;
        //if i>1, there is an expression before the operator
        if (i > 1) {
            temp_vector.clear();
            while (j < i) {
                string curEx = conditions.at(j);
                //if its not a number, and not an expression, its var.
                // turn it to num
                if ((!is_number(curEx)) && (!isExp(curEx))) {
                    it = mapOfVars->find(curEx);
                    curEx = std::to_string(it->second.second);
                }
                temp_vector.push_back(curEx);
                j++;
            }
            if (j == i) {
                firstC = e->evaluate(temp_vector);
            }
        }
        op = conditions.at(i);
        i++;
        if (isOperator(conditions.at(i))) { //if its operator like "<="
            op += conditions.at(i);
            i++;
        }
        j = 0;
        //an temp int to know where the operator is.
        int m = i;
        while (i < condVsize) {
            j++;
            i++;
        }
        //if j>1, there is an expression after the operator
        if (j > 1) {
            temp_vector.clear();
            while (m < condVsize) {
                string curEx = conditions.at(m);
                //if its not a number, turn it to num
                if ((!is_number(curEx)) && (!isExp(curEx))) {
                    it = mapOfVars->find(curEx);
                    curEx = std::to_string(it->second.second);
                }
                temp_vector.push_back(curEx);
                m++;
            }
            secondC = e->evaluate(temp_vector);
        }
    }
    if (vector[curIndex] == "$") //if the line was "while .... {"
        curIndex++;
    //if the line was "while ...." and "{" was in the next line
    if (vector[curIndex] == "{")
        curIndex += 2;
    // if the conditions size is 3 or 4
    if (condVsize == 3 || condVsize == 4) {
        //now we have to convert between strings to double or int
        //if its a number, convert it into double.
        if (is_number(conditions.at(0))) {
            firstC = double(std::stoi(conditions.at(0)));
        } else {     //if its var, find the var value in the map
            it = mapOfVars->find(conditions.at(0));
            firstC = it->second.second;
        }
        if (is_number(conditions.at(condVsize - 1))) {
            secondC = double(std::stoi(conditions.at(condVsize - 1)));
        } else {
            it = mapOfVars->find(conditions.at(condVsize - 1));
            secondC = it->second.second;
        }
        //if the op is >,<
        if (condVsize == 3)
            op = conditions.at(1);
        else
            op = conditions.at(1) + conditions.at(2);
    }
    //check the if condition
    bool doTheCommands = checkCondition(firstC,secondC,op);
    if (doTheCommands) {
        std::vector<string> commands;
        makeCommandsVector(vector,commands,curIndex);
        //do the commands in loop
        std::vector<string> lineOfCommands;
        doIfCommand(commands,lineOfCommands);
        return curIndex-index;
    }
    return curIndex-index;
}
/**
 * make the vector of the comands
 * @param v the original vector
 * @param commands ref to the new vector of commands
 * @param curIndex the current index by ref
 */
void IfCommand::makeCommandsVector(const vector<string> &v, vector<string> &commands, int &curIndex) {
    bool loopInLoop = false;
    while ((v[curIndex] == "$") || (v[curIndex] == "{")) {
        curIndex++;
    }
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
        commands.push_back("EndIf");
        curIndex += 2;
    }
}

/**
 * checking if the conditions are true or false.
 * @param a right exp
 * @param b left exp
 * @param op the operator
 * @return true if its ond false otherwise
 */
bool IfCommand::checkCondition (double a, double b, string op) {
    if (op == "<")
        return a<b;
    if (op == ">")
        return a>b;
    if (op == "<=")
        return a<=b;
    if (op == ">=")
        return a>=b;
    if (op == "!=")
        return a!=b;
    if (op == "==")
        return a==b;
    else
        throw "uncorrect operator";
}

/**
 * do the if command. seperate the lines in the commands vector and execute each of them.
 * @param commands vector of the commands
 * @param lineCommands ref to line vector
 */
void IfCommand::doIfCommand(const vector<string> &commands, vector<string> &lineCommands) {
    int i = 0;
    bool ifOrWhile = true;
    //until we got to the end of the if command
    while (commands[i] != "EndIf") {
        //while the line is ending
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
void IfCommand::executeLoop(vector<string> &lineCommands) {
    int size = lineCommands.size();
    if (size > 1) {
        //if the line command end with }$
        if (lineCommands.at(size - 2) == "}") {
            lineCommands.at(size - 2) = "$";
            lineCommands.pop_back();
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
bool IfCommand::is_number(const std::string &s){
    char *end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end!= s.c_str() && val != HUGE_VAL;
}
/**
 * check if the string is operator
 * @param s string
 * @return true if its operator and false otherwise
 */
bool IfCommand::isOperator (string op) {
    return op == "<"||op == ">"||op == "="||op == "!";
}
/**
 * check if the string is expresion
 * @param s string
 * @return true if its expresion and false otherwise
 */
bool IfCommand::isExp(string exp) {
    return exp == "+" ||exp == "-" ||exp == "*" ||exp == "/";
}

IfCommand::~IfCommand() {

}
