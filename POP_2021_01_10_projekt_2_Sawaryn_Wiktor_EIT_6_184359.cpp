//POP_2020_12_08_projekt_1_Sawaryn_Wiktor_EIT_6_184359_CLion 2020.3.1 x64_Runtime version: 11.0.9.1+11-b1145.63 amd64_MinGW64
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;
//deklaracja sta³ych
const int MAX_STACK_COUNT = 100;
const char possibleChars[] = {' ','0','1','2','3','4','5','6','7','8','9','-','+','*','^','/','(',')','.',','};
const char numbers[] = {'0','1','2','3','4','5','6','7','8','9',',','.'};

//funkcja zwraca wagê danego operatora
int operatorValue(char c){
    switch (c)
    {
        case '+'     : ;
        case '-'     : return 1;
        case '*'     : ;
        case '/'     : return 2;
        case '^'     : return 3;
        default      : return 0;
    }
}

//funkcja oblicza wyra¿enie dwóch liczb
double calculate(double v1, double v2, char op){
    switch (op)
    {
        case '+':
            v1 += v2;
            break;

        case '-':
            v1 -= v2;
            break;

        case '*':
            v1 *= v2;
            break;

        case '/':
            v1 /= v2;
            break;

        case '^':
            v1 = pow(v1,v2);
            break;
    }
    return v1;
}
//funkcja sprawdza czy dany znak do czêœæ liczby
bool IsANumber(char c){
    for(int i = 0; i<12 ; i++) {
        if(numbers[i] == c) return true;
    }
    return false;
}
//funkcja bierze wyra¿enie algebraiczne i zamienia je w wyra¿enie ONP
string ONP(string expression){
    int sCount = 0;
    int bracketBalance = 0;
    bool bracket = false;
    char op = ' ';
    char lastOp = '^';
    string stack[MAX_STACK_COUNT];
    string value;
    string subExpression = "#";

    for(int i = 1 ; i<expression.length() ; i++){
        if(bracket){
            if(expression[i] == '(') bracketBalance++;
            if(expression[i] == ')') bracketBalance--;
            if(bracketBalance == 0){
                bracket = false;
                stack[sCount] = ONP(subExpression+"=");
                sCount++;
                subExpression="#";
            }
            else subExpression = subExpression + expression[i];
        }
        else {
            if(expression[i] != ' '){
                if(expression[i] == '('){
                    bracket = true;
                    bracketBalance++;
                }
                else {
                    if(IsANumber(expression[i])) {
                        value = value + expression[i];
                    }
                    else{
                        stack[sCount] = value + "|";
                        sCount++;
                        if(op != ' '){
                            stack[sCount] = op;
                            stack[sCount] = stack[sCount] + "|";
                            if(operatorValue(op)>operatorValue(lastOp)){
                                int index;
                                stack[sCount] = lastOp;
                                stack[sCount] = stack[sCount] + "|";
                                for(int i = sCount-1; i>=0 ; i--) if(stack[i] == stack[sCount]) {index = i ; break;}
                                for(int i = index; i<sCount ; i++) stack[i] = stack[i+1];
                            }
                            sCount++;
                            lastOp = op;
                        }
                        op = expression[i];
                        value = "";
                    }
                }
            }
        }
    }
    string ansONP;
    for(int i = 0 ; i<sCount ; i++) cout<<"stack :" <<stack[i]<<endl;
    for(int i = 0 ; i<sCount ; i++) {
        if(stack[i] != "|") ansONP = ansONP + stack[i];
    }
    return ansONP;
}

//funcja na podstawie wyra¿enia ONP oblicza wartoœæ wyra¿enia algebraicznego
string answer(string ONPexpresion){
    cout << ONPexpresion<<endl;
    double stack[MAX_STACK_COUNT];
    int sCount = 0;
    string value;
    for(int i ; i<ONPexpresion.length() ; i++){
        if(IsANumber(ONPexpresion[i])){
            value = value + ONPexpresion[i];
        }
        else if (ONPexpresion[i] == '|' && IsANumber(ONPexpresion[i-1])){
            cout << stod(value) <<endl;
            stack[sCount++] = stod(value);
            value = "";
        }
        else{
            if(sCount > 1 && ONPexpresion[i] != '|') {
                double v2 = stack[--sCount];
                double v1 = stack[--sCount];
                stack[sCount++] = calculate(v1, v2, ONPexpresion[i]);
            }
        }
    }
    return to_string(stack[0]);
}

//funkcja bierze wyra¿enie algebraiczne podane w pliku, nastênie zwraca to samo wyra¿enie wraz z odpowiedzi¹
string calculateExpression(string expression){
    if(expression[0] == '#' && expression[expression.length()-1] == '='){
        for(int i = 1; i<expression.length()-1 ; i++){
            for(int j = 0; j<20 ; j++) {
                if (expression[i] == possibleChars[j]) break;
                if(j == 19) return " err";
            }
        }
        string ONPex = ONP(expression);
        return " " + answer(ONPex);
    }
    else return " err";
}
int main() {
    ifstream FileToRead;
    string operationsList;  //wszystkie wyra¿enia wraz z odpowiedziami bêd¹ dodawane do tego stringa
    string expression;
    FileToRead.open("C:\\Users\\Wiktor\\Desktop\\POP2\\operations.txt"); //otworzenie pliku najpierw w trybie odczytu
    if(!FileToRead){
        cout << "Plik FileToRead nie zostal otworzony"<<endl;
    }
    else{
        cout << "Plik FileToRead zostal otworzony"<<endl;
        while(!FileToRead.eof()){
            getline(FileToRead,expression);
            if(expression !="") {
                string ans = calculateExpression(expression);
                operationsList = operationsList + expression + ans + '\n';
            }
        }
        FileToRead.close();
        ofstream FileToWrite;
        //otwieramy ten sam plik w trybie zapisu z w³aœciwoœci¹ "trunc", ¿eby móc zapisaæ wszystkie wyra¿enia wraz z odpowiedziami zapisanymi w operationsList
        FileToWrite.open("C:\\Users\\Wiktor\\Desktop\\POP2\\operations.txt", ios::trunc);
        if(!FileToWrite){
            cout<<"Plil FileToWrite nie zostal otwarty"<<endl;
        }
        else{
            cout << "Plik FileToWrite zostal otwarty" <<endl;
            FileToWrite << operationsList;
            FileToWrite.close();
        }
    }
    return 0;
}
