
#include "PrintCommand.h"

using namespace std;
/**
 * constructor
 * @param varCommand ref to define var connamd
 * @param commandsMap map of the commands
 */
PrintCommand::PrintCommand(DefineVarCommand *varCommand, map<string, Command *> commandsMap) {
    this->commands = commandsMap;
    this->dfc = varCommand;
}

/**
     * do the command
     * @param vector the vector of the skript
     * @param index the  index of the current command
     * @return int, number of steps for the next command in the vector
     */
int PrintCommand::doCommand(const vector<string> &vector, int index) {
    std::vector<string> tempVec;
    this->mapOfVars = this->dfc->getMap();
    map<string, pair<string, double>>::iterator it;
    int temp = index+1;
    int counter = 0;
    //check how long the expression is.
    while (vector.at(temp) != "$"){
        counter++;
        temp++;
    }
    if (vector.at(index+1).find("\"") == 0)
    {
        printString(vector.at(index+1));
        return counter +1;
    }
    int tempConvert = index+1;
    string stringToConvert;
    for (int i = 0; i< counter ; i++){
        //if the string is a number
        if (is_number(vector.at(tempConvert))){
            stringToConvert = stringToConvert+ ' ' + vector.at(tempConvert);
            tempVec.push_back(vector.at(tempConvert));
            tempConvert++;
        }
            //check if the string is an aperator
        else if ((vector.at(tempConvert) =="+") || (vector.at(tempConvert) =="-") ||(vector.at(tempConvert) =="*") ||
                 (vector.at(tempConvert) =="/") || (vector.at(tempConvert) =="(") || (vector.at(tempConvert) ==")")){
            stringToConvert = stringToConvert+ ' ' + vector.at(tempConvert);
            tempVec.push_back(vector.at(tempConvert));
            tempConvert++;
        }
            //if the string is a parameter
        else{
            it = mapOfVars->find(vector.at(tempConvert));
            // CHANGE TO DOUBLE!!!
            double valueToAssign =  it->second.second;
            string doubleToConvert = std::to_string(valueToAssign);
            stringToConvert = stringToConvert + doubleToConvert;
            tempVec.push_back((doubleToConvert));
            tempConvert++;

        }
    }
    EqualsCommand *c = (EqualsCommand*) commands["="];
    string newString = std:: to_string((c->evaluate(tempVec)));
    //print the string
    printString(newString);
    return counter+1;
}
/**
 * check if the strind is a number. if it is its return true and false othewise
 */
bool PrintCommand::is_number(const std::string &s){
    char *end = 0;
    double val = strtod(s.c_str(), &end);
    return end!= s.c_str() && val != HUGE_VAL;

}
/**
 * printing the curren string
 */
void PrintCommand::printString(string s){
    cout << s << "\n";
}
