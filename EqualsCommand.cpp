#include "EqualsCommand.h"
using namespace std;

/**
 * constructor
 * @param varCommand the current var command witch hold the map op vars
 * @param commands map of commands
 */
EqualsCommand::EqualsCommand(DefineVarCommand *varCommand, map<string, Command*> commands) {
    this->dfc = varCommand;
    this->commandMap = std::move(commands);
}
/**
 * do command.
 * @param vector vector of the script.
 * @param index current index of the equalls command
 * @return number of steps for the next command
 */
int EqualsCommand::doCommand(const vector<string> &vector, int index) {
    //get the connect command from the map
    this->cnctCmd = (ConnectCommand*)this->commandMap["connect"];
    //from this connect command get the current connect to server.
    connectToServer = cnctCmd->ds;
    //create a new vector
    std::vector<string> tempVec;
    //get the map from the dfs.
    this->mapOfVars = this->dfc->getMap();
    map<string, pair<string, double>>::iterator it;


    //format of var x = bind "/controls/flight/speedbreak"
    if (vector.at(index + 1) == "bind") {
        string check = vector.at(index+2);
        if (check.find('"') == 0) {
            string object = vector.at(index - 1);
            string path = vector.at(index + 2);
            this->mapOfVars->insert(make_pair(object, make_pair(path, 0)));
            return 4;
        }
            //format of "var x = bind b"
        else {
            string toAssign = vector.at(index + 2);
            string pathToAssign = mapOfVars->find(toAssign)->second.first;
            this->mapOfVars->insert(make_pair(vector.at(index-1), make_pair(pathToAssign, 0)));
            return 4;
        }
    }

    //if its not a bind
    int temp = index + 1;
    int counter = 0;
    //check how long the expression is.
    while (vector.at(temp) != "$") {
        counter++;
        temp++;
    }
    int tempConvert = index + 1;
    string stringToConvert;
    for (int i = 0; i < counter; i++) {
        if (is_number(vector.at(tempConvert))) {
            stringToConvert = stringToConvert + ' ' + vector.at(tempConvert);
            tempVec.push_back(vector.at(tempConvert));
            tempConvert++;
        } else if ((vector.at(tempConvert) == "+") || (vector.at(tempConvert) == "-") || (vector.at(tempConvert) == "*") ||
                   (vector.at(tempConvert) == "/") || (vector.at(tempConvert) == "(") || (vector.at(tempConvert) == ")")) {
            stringToConvert = stringToConvert + ' ' + vector.at(tempConvert);
            tempVec.push_back(vector.at(tempConvert));
            tempConvert++;
        } else {
            it = mapOfVars->find(vector.at(tempConvert));
            double valueToAssign = it->second.second;
            string doubleToConvert = std::to_string(valueToAssign);
            stringToConvert = stringToConvert + ' ' + doubleToConvert;
            tempVec.push_back(doubleToConvert);
            tempConvert++;
        }
    }
    string newString = std:: to_string((evaluate(tempVec)));
    //format of "breaks = 0" and var "breaks = 1"
    if (index >= 2) {
        //if we're adding a new var with no path
        if (vector.at(index - 2) == "var") {
            this->mapOfVars->insert(make_pair(vector.at(index - 1), make_pair(" ", atof(newString.c_str()))));
            return counter+1;
        }
    }
        //if the index is < 2
        it = mapOfVars->find(vector.at(index - 1));
        //update map
        it->second.second = atof(newString.c_str());
        //get the path to send to the server
        string pathToSend = it->second.first.substr(1, it->second.first.size()-2);
        string toSend = "set " + pathToSend + " " + newString + "\r\n";

        //connect to the server and write to it.

        this->connectToServer->executeConnectServer(toSend);

        return counter+1;

}


/**
 * Function to find precedence of operators.
 * @param op the op
 * @return 1 if its +- or 2 if its / OR *
 */
int EqualsCommand::precedence(string op){
    if(op == "+"||op == "-")
        return 1;
    if(op == "*"||op == "/")
        return 2;
    return 0;
}

/**
 * Function to perform arithmetic operations.
 * @param a left expression
 * @param b right expression
 * @param op the operator
 * @return new expression
 */
Expression* EqualsCommand::applyOp(Expression* a, Expression* b, string op){
    if (op == "-"){
        return new Minus(a, b);
    }
    else if (op == "*"){
        return new Mul(a,b);
    }
    else if (op == "/"){
        return new Divi(a,b);
    }
    else if(op == "+"){
        return new Plus(a,b);
    }
    else {
        return nullptr;
    }
}

/**
 * Function that returns value of expression after evaluation.
 * @param old_vector vector of strings of the expression.
 * @return value after calculating
 */

double EqualsCommand::evaluate(const vector<string> &old_vector){
    vector<string> tokens;
    //handles casses that the number is negative
    zeroMinus(old_vector,tokens);
    int i;

    // stack to store integer values.
    stack <double> values;

    // stack to store operators.
    stack <string> ops;
    int size = tokens.size();
    for(i = 0; i < size; i++){

        // Current token is a whitespace,
        // skip it.
        if(tokens.at(i) == " ")
            continue;

            // Current token is an opening
            // brace, push it to 'ops'
        else if(tokens.at(i) == "("){
            ops.push(tokens.at(i));
        }

            // Current token is a number, push
            // it to stack for numbers.
        else if(is_number(tokens.at(i))){
            double stringAsDouble = atof(tokens.at(i).c_str());
            values.push(stringAsDouble);
        }

            // Closing brace encountered, solve
            // entire brace.
        else if(tokens.at(i) == ")")
        {
            while(!ops.empty() && ops.top() != "(")
            {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                string op = ops.top();
                ops.pop();

                Expression* a = new Number(val1);
                Expression* b = new Number(val2);
                values.push(applyOp(a,b,op)->calculate());
            }

            // pop opening brace.
            ops.pop();
        }

            // Current token is an operator.
        else
        {
            // While top of 'ops' has same or greater
            // precedence to current token, which
            // is an operator. Apply operator on top
            // of 'ops' to top two elements in values stack.
            while(!ops.empty() && precedence(ops.top())
                                  >= precedence(tokens[i])){
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                string op = ops.top();
                ops.pop();

                Expression* a = new Number(val1);
                Expression* b = new Number(val2);
                values.push(applyOp(a,b,op)->calculate());;
            }

            // Push current token to 'ops'.
            ops.push(tokens.at(i));
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
    while(!ops.empty()){
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        string op = ops.top();
        ops.pop();
        Expression* a = new Number(val1);
        Expression* b = new Number(val2);
        values.push(applyOp(a,b,op)->calculate());
    }

    // Top of 'values' contains result, return it.
    return values.top();
}
/**
 * adding an 0 before minus in the old vector and push it into new vector.
 * @param old_vector the original vector
 * @param tokens the vector with zero
 */
void EqualsCommand::zeroMinus (const vector<string> &old_vector, vector<string> &tokens) {
    int oldVectorSize = old_vector.size();
    bool paran = false;
    bool bigParan = false;
    bool in = true;
    bool prev_num = false;
    bool exit = false;
    for (int i = 0; i < oldVectorSize; i++) {
        exit = false;
        in = true;
        if (old_vector[i] == "-") {
            //check if its double minus
            if (old_vector[i + 1] == "-") {
                //if it is , turn it to plus if it after number, or delete the --
                // if it the first value or value after exp
                if ((i == 0) || isExp(old_vector[i-1])) {
                    i+=1;
                } else {
                    tokens.push_back("+");
                    i+=1;
                }
            } else {        //if its not double minus
                //if the previous value was a number.
                if (i > 0) {
                    if ((is_number(old_vector[i - 1]))) {
                        tokens.push_back("-");
                        prev_num = true;
                    }
                    if (!(is_number(old_vector[i - 1])))
                        prev_num = false;
                    //if it was an expression (exp)-a
                    if ((old_vector[i - 1] == ")")) {
                        tokens.push_back("-");
                        in = false;
                    }
                }
                if (((i > 0) && in && (!prev_num)) ||
                    (i == 0)) {   //if its not double minus and there is no number before the minus.
                    //if its : "-("
                    //check if is -(a) if it is turn it to +(0-a)
                    if (old_vector[i + 1] == "(") {
                        paran = true;
                        if ((is_number(old_vector[i + 2])) && (old_vector[i + 3] == ")")) {
                            tokens.push_back("(");
                            tokens.push_back("0");
                            tokens.push_back("-");
                            tokens.push_back(old_vector[i + 2]);
                            tokens.push_back(")");
                            paran = false;
                            exit = true;
                            i += 4;
                        }
                    }
                    //if its the first value in the vector
                    if (i == 0) {
                        tokens.push_back("(");
                        tokens.push_back("0");
                        tokens.push_back("-");
                        if (!paran) { //if its not a parantaties its have to be number
                            tokens.push_back(old_vector[i + 1]);
                            tokens.push_back(")");
                            i++;
                        }
                    } else if ((i > 0) && (!exit)){ //if i is not 0
                        if (old_vector[i - 1] == "(") {
                            //if its expr like (-a exp b)
                            if (old_vector[i + 2] != ")") {
                                tokens.push_back("(");
                                paran = false;
                            }
                            tokens.push_back("0");
                            tokens.push_back("-");
                        }
                            //if its not "(" or ")" and not a number its have to be another parm
                        else {
                            tokens.push_back("(");
                            tokens.push_back("0");
                            tokens.push_back("-");
                        }
                        if (!paran) { //if its not a parantaties its have to be number
                            tokens.push_back(old_vector[i + 1]);
                            tokens.push_back(")");
                            i++;
                        }
                    }
                }
            }
        } else {   //if its not minus
            if (old_vector[i] != ")")
                tokens.push_back(old_vector[i]);
            if (old_vector[i] == ")") {
                if (paran) {
                    tokens.push_back(")");
                    paran = false;
                } if (bigParan) {
                    if (old_vector[i-3] != "(") {
                        tokens.push_back(")");
                        bigParan = false;
                    }

                }
            }
            if ((old_vector[i] == "(") && !bigParan)
                bigParan = true;
        }
    }
}
/**
 * check if the string is a number
 * @param s the string
 * @return true if ita a number and false otherwise
 */
bool EqualsCommand::is_number(const string &s){
    char *end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end!= s.c_str() && val != HUGE_VAL;
}
/**
 * check if the current string is an expression
 * @param exp the current string
 * @return true if ita an expression and false otherwise
 */
bool EqualsCommand::isExp(const string &exp) {
    return exp == "+" ||exp == "-" ||exp == "*" ||exp == "/";
}



