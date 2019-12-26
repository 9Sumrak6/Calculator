#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Parser{
	string expr;
	bool IsBracket(char a);
	bool IsOperator(char a);
	bool IsDigit(char a);
	bool IsLetter(char a);
public:
	Parser(string expression);
	vector<string> Parse();
};

Parser::Parser(string expression){
   	expr = expression;
}

bool Parser::IsBracket(char a){
	return a == '(' || a == ')';
}

bool Parser::IsOperator(char a){
	return a == '+' || a == '-' || a == '*' || a == '/' || a == '^';
}

bool Parser::IsDigit(char c){
	return c >= '0' && c <= '9'; 
}

bool Parser::IsLetter(char c){
	return (c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z');
}

vector<string> Parser::Parse(){
	vector<string> str;
	int i = 0;

	while(i < expr.length()){
		if(IsBracket(expr[i]) || IsOperator(expr[i]) || expr[i] == '='){
			str.push_back(string (1, expr[i]));
			i++;
		}
		else if(IsLetter(expr[i])){
			string id = "";
			while(i < expr.length() && (IsLetter(expr[i]) || IsDigit(expr[i]))){
				id += expr[i];
				i++;
			}
			str.push_back(id);
		}
		else if(IsDigit(expr[i])){
			string num =  "";
			int k = 0;
			while (i < expr.length() && (IsDigit(expr[i]) || expr[i] == '.')){
				num += expr[i];
				if (expr[i] == '.')
					k++;
				if (k >= 2){
					throw string("incorrect number: ") + num;
				}
				i++;
			}	

			str.push_back(num);
		}

		else if(expr[i] == ' ')
			i ++;

		else {
			throw string("Error: unknown character");
		}
	}

	return str;
}