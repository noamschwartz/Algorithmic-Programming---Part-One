
#include "Interpreter.h"
#include "PrintCommand.h"
#include "SleepCommand.h"

using namespace std;
/**
 * constructor
 * @param fileName name of the script flie.
 */
Interpreter::Interpreter(string name) {
    this->filename = name;
}
/**
 * make the map of the commands
 * @return map of commands
 */
map<string, Command*> Interpreter::makeMap (){
    map<string, Command*> commands;
    DefineVarCommand* dfc = new DefineVarCommand();
    ConnectCommand* connectCommand = new ConnectCommand();
    commands.insert(pair<string, Command*> ("openDataServer", new OpenServerCommand(dfc)));
    commands.insert(pair<string,Command*> ("connect", connectCommand));
    commands.insert(pair<string,Command*> ("var", (Command*)dfc));
    commands.insert(pair<string,Command*> ("=", new EqualsCommand(dfc, commands)));
    commands.insert(pair<string,Command*> ("while", new LoopCommand(dfc,commands)));
    commands.insert(pair<string,Command*> ("if", new IfCommand(dfc,commands)));
    commands.insert(pair<string,Command*> ("print", new PrintCommand(dfc, commands)));
    commands.insert(pair<string, Command*> ("sleep", new SleepCommand()));
    commands.insert(pair<string, Command*> ("EndOfText", new exitCommand(&commands)));

    return commands;
}

/**
 * execude commands in the script
 * @param vector every value in the vector is a word ib the script.
 */
void Interpreter::parser(vector<string> vector) {
    //size of the vector
    std::vector<string>::size_type size = vector.size();
    std::vector<thread* > threads ;
    //map of commands
    map<string, Command*> mapOfCommands= makeMap();
    int index = 0;
    Command* c = NULL;
    while (vector.at(index) != "EndOfText") {
        //get the appropriate command from the map
        string key = vector[index];
        c = mapOfCommands[key];

        //if there is a appropriate command in the map.
        if (c != NULL){
            //send the index of the command and a copy of the remainder of the vector.
            index += c->doCommand(vector, index);


        }
         //if there is no command like this, throw an error
        else {
            index++;
        }
    }
    //if it the end of the text, do the exit command.
    if(vector.at(index) == "EndOfText") {
        c = mapOfCommands["EndOfText"];
        index+= c->doCommand(vector, index);
    }
}
/**
 * seperate the script to vector by space
 */
void Interpreter::lexer(){
    vector<string> v;
    ifstream file;
    string line;
    string word;
    file.open(this->filename);
    if (file.is_open()){
        while(getline(file, line)){
            stringstream wordStreamer(line);
            while(getline(wordStreamer, word, ' ')) {
                if(word!=""){
                    v.push_back(word);
                }
            }
            //push $ at the end of the line
            v.push_back("$");
        }
    }
    //push endoftext at the end of the vector
    v.push_back("EndOfText");
    //seperate the text , every value in the new vector will be only one word or num or operator
    v = lexserHelp(v);
    //send the vector to parser
    parser(v);
 }

/**
 * seperat the text , every value in the new vector will be only one word or num or operator
 * @param originalV the vector from lexer function
 * @return new seperated vector
 */
vector<string> Interpreter::lexserHelp(const vector<string> &originalV) {
    vector<string> newVector;
    vector<string> lineV;
    vector<int> parmPlace;
    double curInd = 0;
    while (originalV.at(curInd) != "EndOfText") {
        lineV.clear();
        while (originalV.at(curInd) != "$") {
            //create vector for line in text
            lineV.push_back(originalV.at(curInd));
            curInd++;
        }
        curInd++;
        int size = lineV.size();
        //now we have line from the text.
        //check if we have to separate it.
        for (int i = 0; i < size; i++) {
            string currentC = lineV.at(i);
            int curCsize = currentC.size();
            if (currentC == "bind") {
                int sizeOfLine = lineV.size();
                newVector.push_back(currentC);
                i++;
                while (i < sizeOfLine) {
                    newVector.push_back(lineV.at(i));
                    i++;
                }
                break;
            }
            //if the string is only a operator
            if (curCsize == 1) {
                newVector.push_back(currentC);
            } else {
                parmPlace.clear();
                //for each string in the vector find the operators inside
                whereTheOp(currentC, parmPlace);
                //if there is no operator in the string
                if (parmPlace.empty()) {
                    newVector.push_back(currentC);
                } else {
                    int parmVecSize = parmPlace.size();
                    //if there is only one op in the string
                    if (parmVecSize == 1) {
                        //if the prm is in the beginning of the string
                        if (parmPlace[0] == 0) {
                            newVector.push_back(currentC.substr(0, 1));
                            newVector.push_back(currentC.substr(1));
                        }
                        //if the prm is in the end of the string
                        if (parmPlace[0] == curCsize-1) {
                            newVector.push_back(currentC.substr(0, curCsize-1));
                            newVector.push_back(currentC.substr(curCsize-1));
                        }
                        if ((0 < parmPlace[0]) && (parmPlace[0] < (curCsize-1))) {
                            newVector.push_back(currentC.substr(0, parmPlace[0]));
                            newVector.push_back(currentC.substr(parmPlace[0],1));
                            newVector.push_back(currentC.substr(parmPlace[0]+1, curCsize));
                        }
                    } else {
                        //for each parm push the string before the parm to the vect and the parm
                        string temp;
                        for (int j = 0; j < parmVecSize; j++) {
                            temp.clear();
                            //check the first parmPlace
                            if (j == 0) {
                                temp = currentC.substr(0, parmPlace[0]);
                                if (!temp.empty()) {
                                    if (temp.size() > 1)
                                        newVector.push_back(temp);
                                    else {
                                        if (!checkIfOp(temp.at(0)))
                                            newVector.push_back(temp);
                                    }
                                }
                                newVector.push_back(currentC.substr(parmPlace[0],1));
                            } else {    //if this is not the first place
                                int begin = parmPlace[j-1]+1;
                                int endOfS = (parmPlace[j] - parmPlace[j-1] - 1);
                                if (endOfS < 1) {
                                    endOfS = 1;
                                }
                                temp = currentC.substr(begin, endOfS);
                                if (!temp.empty()) {
                                    if (temp.size() > 1)
                                        newVector.push_back(temp);
                                    else {
                                        if (!checkIfOp(temp.at(0)))
                                            newVector.push_back(temp);
                                    }
                                }
                                newVector.push_back(currentC.substr(parmPlace[j], 1));
                            }
                        }
                        //if is not the end of the word
                        int first = parmPlace[parmVecSize - 1];
                        int second = currentC.size()-1;
                        if (first != second) {
                            temp = currentC.substr(parmPlace[parmVecSize-1] + 1);
                            //push the rest of the string into the vector
                            if (!temp.empty()) {
                                if (temp.size() > 1)
                                    newVector.push_back(temp);
                                else {
                                    if (!checkIfOp(temp.at(0)))
                                        newVector.push_back(temp);
                                }
                            }
                        }
                    }
                }
            }
        }
        newVector.push_back("$");
    }
    newVector.push_back("EndOfText");
    return newVector;
}

/**
 * check where the operators in the word
 * @param s the strinf
 * @param v vector of int in witch we push the places of the operators
 */
void Interpreter::whereTheOp(string s, vector<int> &v) {
    int size = s.size();
    for (int i = 0; i < size; i++) {
        if (checkIfOp(s.at(i))) {
            v.push_back(i);
        }
    }
}
/**
 * check if the char is operator
 * @param c the current char
 * @return true if it operator and false othewise
 */
bool Interpreter::checkIfOp(char c) {
    return c == '+' || c == '-' || c == '(' || c == ')' || c == '/' || c == '*' ||
    c == '=' || c == '!' || c == '<' || c == '>'|| c == '{'|| c == '}';
}
