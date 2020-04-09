#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <vector>
#include <stack>
#include "Stack.hpp"
#include "Parser.hpp"

using namespace std;

class Calc{
	struct Variable{
		string name;
		double value;
	};
	
	struct UserFunction{
		string name;
		string arg;
		vector<string> pol;
	};

	string status = "rad";
	
	vector<Variable> variables;
	vector<UserFunction> functions;
	
	int GetVariableIndex(string name);
	void AddVariable(string name, double value);
	double GetVariableValue(string name);
	bool IsVariable(string name);
	
	int GetUserFunctionIndex(string name);	
	bool IsNumber(string num);
	bool IsOperator(string op);
	bool IsFunc(string fun);
	bool IsUserFunc(string fun);
	bool IsIdentifier(string var);

	double CalcFunction(string op, double a);
	double CalcUserFunction(string op, double a);
	double CalcOperator(string op, double a, double b);
	double CalcConst(string name);
	double Calculate(vector<string> &rpn);
	
	int Priority(string op);
	
	bool IsConst(string name);

	void State();
	bool IsAdd(string lexeme, string lexemes1);
	void ProcessAdd(vector<string> &lexemes);
	void ProcessSet(vector<string> &lexemes);
	void ProcessMode(vector<string> &lexemes);
	void ProcessDef(vector<string> &lexemes);
public:	
	void Help();
	vector<string> GetRPN(vector<string> lexemes);
	void ProcessCommand(string cmd);
};	

int Calc::GetVariableIndex(string name) {
	for(int i = 0; i < variables.size(); i++) 
		if (variables[i].name == name)
			return i;
	return -1;
}

void Calc::AddVariable(string name, double value) {
	int index = GetVariableIndex(name);
	
	if (index < 0) {
		Variable var;	
		var.name = name;
		var.value = value;
		variables.push_back(var);
	}
	else 
		variables[index].value = value;
}		

int Calc::GetUserFunctionIndex(string name){
	for (int i = 0; i < functions.size(); i++)
		if (functions[i].name == name)
			return i;
	return -1;
}

bool Calc::IsNumber(string num) {
	for (int i = 0; i < num.length(); i++) {
		if (num[i] != '.' &&  (num[i] < '0' || num[i] > '9' ))
			return false;
	}
	return true;
}

bool Calc::IsOperator(string op) {
	return op == "+" || op == "-" || op == "^" || op == "%" || op == "*" || op == "/"; 
}

bool Calc::IsUserFunc(string str){
	for (int i = 0; i < functions.size(); i++)
		if (str == functions[i].name)
			return true;

	return false;	
}

bool Calc::IsFunc(string op) {
	return op == "cos" || op == "sin" || op == "tan" || op == "cot" || op == "ctg"|| op == "exp" || op == "sqrt";
}

bool Calc::IsIdentifier(string var){
	if (IsFunc(var) || IsConst(var))
		return false;
	
	for(int i = 0; i < var.length(); i++){
		bool isDigit = var[i] >= '0' && var[i] <= '9';
		bool isLetter = (var[i] >= 'a' && var[i] <= 'z') || (var[i] >= 'A' && var[i] <= 'Z');
		if (!isDigit && !isLetter)
			return false;
	}

	return true;
}

bool Calc::IsConst(string name){
	return name == "pi" || name == "e";
}

double Calc::GetVariableValue(string name) {
	int index = GetVariableIndex(name);

	if (index >= 0)
		return variables[index].value;

	throw string("No match found for variable '") + name + "'";
}

bool Calc::IsVariable(string name){
	for (int i = 0; i < variables.size(); i++)
		if (name == variables[i].name)
			return true;

	return false;	
}

double Calc::CalcFunction(string name, double a) {
	if (status == "deg")
		a *= M_PI / 180;
	
	if (name == "cos")
		return cos(a);

	if (name == "sin")
		return sin(a);

	if (name == "tan" || name == "tg")
		return tan(a);
	
	if (name == "cot" || name == "ctg")
		return 1/tan(a);
	
	if (name == "sqrt")
		return sqrt(a);

	if (name == "exp")
		return exp(a);

	throw string("unknown function '") + name + "'";   
}

double Calc::CalcConst(string name) {
	if (name == "pi")
		return M_PI;

	if (name == "e")
		return M_E;

	throw string("unknown constant '") + name + "'";
}

double Calc::CalcUserFunction(string name, double a){
	int index = GetUserFunctionIndex(name);
	vector<string> p;
	for (int i = 0; i < functions[index].pol.size(); i++){
		if (functions[index].pol[i] == functions[index].arg)
			p.push_back(to_string(a));
		else
			p.push_back(functions[index].pol[i]);
	}
	return Calculate(p);
}

double Calc::CalcOperator(string op, double a, double b) {
	if (op == "-")
		return a - b;
	
	if (op == "+")
		return a + b;
	
	if (op == "*")
		return a * b;

	if (op == "/")
		return a / b;

	if (op == "^")
		return pow(a, b);

	if (op == "%")
		return	fmod(a, b);

	throw string("unknown operator '") + op + "'";   
}

double Calc::Calculate(vector<string> &rpn) {
	Stack stack;

	for (int i = 0; i < rpn.size(); i++) {
		if (IsNumber(rpn[i])) {
			stack.Push(stod(rpn[i])); 
		}
		else if (IsConst(rpn[i])){
			stack.Push(CalcConst(rpn[i])); 
		}
		else if (IsOperator(rpn[i])) {
			if (stack.GetSize() < 2) 
				throw string("Error, too little values to calculate operator '") + rpn[i] + "'";

			double a = stack.Top();
			stack.Pop();
			double b = stack.Top();
			stack.Pop();
			stack.Push(CalcOperator(rpn[i], b, a));
		}
		else if (rpn[i] == "!"){
			if (stack.GetSize() < 1)
				throw string("Error, too little values to insert value");

			double top = stack.Top();
			stack.Pop();
			stack.Push(-top);
		}			
		else if (IsFunc(rpn[i])) {
			if (stack.IsEmpty())
				throw string("Error, too little values to calculate func '") + rpn[i] + "'";

			double a = stack.Top();
			stack.Pop();
			stack.Push(CalcFunction(rpn[i], a));
		}
		else if(IsUserFunc(rpn[i])){
			if (stack.IsEmpty())
				throw string("Error, too little values to calculate func '") + rpn[i] + "'";

			double a = stack.Top();
			stack.Pop();
			stack.Push(CalcUserFunction(rpn[i], a));
		}
		else if (IsIdentifier(rpn[i])) {
			stack.Push(GetVariableValue(rpn[i]));
		}
		else {
			throw string("Unknown rpn value '") + rpn[i] + "'";
		}
	}

	if (stack.GetSize() > 1)
		throw string("Error: incorret expression");

	return stack.Top();
}

int Calc::Priority(string op){
	if (op == "^")
		return 5;

	if (op == "!")
		return 4;

	if (op == "*" || op == "/" || op == "%")
		return 3;

	if (op == "+" || op == "-")
		return 2;

	return 1;
}

void Calc::Help(){
	cout << "usage:" << endl;
	cout << "expresison or сommand commands:" << endl;
	cout << "state - print defined variables and functions" << endl;
	cout << "mode [deg|rad] - get current trigonometry mode or set mode to degrees or radians" << endl;
	cout << "set [name] = [expression] - set variable with name 'name' and set evaluated value of expression to it" << endl;
	cout << "def [name]([arg]) = [expression] - definition of user function of one argument" << endl;
}

void Calc::State() {
	if (variables.size() == 0) {
		cout << "No defined variables" << endl;
		return;
	}

	cout << "+--------------------+-------------------+" << endl;
	cout << "|      VARIABLE      |        VALUE      |" << endl;	
	cout << "+--------------------+-------------------+" << endl;
	
	for (int i = 0; i < variables.size(); i++)
		cout << "| " << setw(18) << variables[i].name << " | " << setw(17) << variables[i].value << " |" << endl;

	cout << "+--------------------+-------------------+" << endl;
	/*cout << endl;
	
	cout << "+--------------------+-------------------+" << endl;
	cout << "|      Functions     |        VALUE      |" << endl;	
	cout << "+--------------------+-------------------+" << endl;

	for (int i = 0; i < functions.size(); i++)
		cout << "| " << setw(18) << functions[i].name << " | " << setw(17) << functions[i].value << " |" << endl;

	cout << "+--------------------+-------------------+" << endl;*/
}

bool Calc::IsAdd(string lex1, string lex2){
	if (((IsNumber(lex1) || IsConst(lex1)) && (IsVariable(lex2) || IsUserFunc(lex2) || IsFunc(lex2) || lex2 == "(")))
		return true;

	if (lex1 == ")" && lex2 == "(")
		return true;

	if (IsVariable(lex1) && IsVariable(lex2))
		return true;

	return false;
}

void Calc::ProcessAdd(vector<string> &lexemes){
	for (int i = 0; i < lexemes.size() - 1; i++)
		if (IsAdd(lexemes[i], lexemes[i + 1]))
			lexemes.insert(lexemes.begin() + i + 1, "*");
}

void Calc::ProcessSet(vector<string> &lexemes){
	if (lexemes.size() < 3)
		throw string("Invalid 'set' operation"); 

	if (!IsIdentifier(lexemes[1]))
		throw string("'") + lexemes[1] + "' is not variable name"; 
		
	string name = lexemes[1];
	lexemes.erase(lexemes.begin(), lexemes.begin() + 2);
	vector<string> rpn = GetRPN(lexemes);
	AddVariable(name, Calculate(rpn));
}

void Calc::ProcessMode(vector<string> &lexemes){
	if (lexemes.size() < 1 || lexemes.size() > 2)
		throw string("Invalid 'mode' operation"); 
		
	if (lexemes.size() == 1) {
		cout << status << endl;
		return;
	}
	
	if (lexemes[1] == "deg" || lexemes[1] == "rad") {
		status = lexemes[1];
		return;
	}

	throw string("Invalid 'mode' operation"); 
}

void Calc::ProcessDef(vector<string> &lexemes){
	if (lexemes.size() < 7)
		throw string("Invalid 'def' operation");
	
	if (!IsIdentifier(lexemes[1]))
		throw string("'") + lexemes[1] + "' is incorrect function name";

	for(int i = 0; i < functions.size(); i++)
		if (functions[i].name == lexemes[1])
			throw string("Function '") + lexemes[1] + "' was already declared";

	if (!IsIdentifier(lexemes[3]))
		throw string("Invalid arg in function definition");

	if (lexemes[2] != "(" || lexemes[4] != ")")
		throw string("Brackets were expected");

	if (lexemes[5] != "=")
		throw string("Brackets were expected");

	UserFunction func;
	func.name = lexemes[1];
	func.arg = lexemes[3];

	for (int i = 6; i < lexemes.size(); i++)
		func.pol.push_back(lexemes[i]);

	func.pol = GetRPN(func.pol);

	functions.push_back(func);
}

void Calc::ProcessCommand(string cmd) {
	try{
		Parser parser(cmd);
		vector<string> lexemes = parser.Parse();
		ProcessAdd(lexemes);

		if (lexemes.size() == 0)
			throw string("Expression is empty");

		if (lexemes[0] == "set") {
			ProcessSet(lexemes);
		}
		else if (lexemes[0] == "mode"){
			ProcessMode(lexemes);
		}
		else if (lexemes[0] == "def"){
			ProcessDef(lexemes);
		}
		else if (cmd == "state") {
			State();
		}
		else if (cmd == "help"){
			Help();
		}
		else {
			vector<string> rpn = GetRPN(lexemes);
			cout << Calculate(rpn) << endl;
		}
	}
	catch (string e){
		cout << e << endl;
	}
}

vector<string> Calc::GetRPN(vector<string> lexemes){
	vector<string> rpn;
	stack<string> stack;
	bool IsUnary = true;

	for (int i = 0; i < lexemes.size(); i++){
		if (IsFunc(lexemes[i]) || IsUserFunc(lexemes[i])) {
			stack.push(lexemes[i]);
			IsUnary = true;
		}
		else if (IsNumber(lexemes[i]) || IsIdentifier(lexemes[i]) || IsConst(lexemes[i])) {
			rpn.push_back(lexemes[i]);
			IsUnary = false;
		}
		else if (lexemes[i] == "(") {	
			stack.push(lexemes[i]);
			IsUnary = true;
		}
		else if (lexemes[i] == ")") {
			while (!stack.empty() && stack.top() != "(") {
				rpn.push_back(stack.top());
				stack.pop();
			}

			if(stack.empty())
				throw string("Incorrect expression");

			stack.pop();
			IsUnary = false;
		}
		else if (IsOperator(lexemes[i])) {
			while (!stack.empty() && (IsFunc(stack.top()) || IsUserFunc(stack.top()) || (Priority(stack.top()) > Priority(lexemes[i])))) {
				rpn.push_back(stack.top());
				stack.pop();
			}

			if (IsUnary && lexemes[i] == "-")
				stack.push("!");
			else
				stack.push(lexemes[i]);

			IsUnary = false;
		}
	}
	
	while(!stack.empty()) {
		if(!IsFunc(stack.top()) && !IsUserFunc(stack.top()) && !IsOperator(stack.top()) && stack.top() != "!")
			throw string("Incorrect expression");

		rpn.push_back(stack.top());
		stack.pop();
	}

	return rpn;
}