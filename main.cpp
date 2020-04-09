#include <iostream>
#include <string>
#include "Calculator.hpp"

using namespace std;

int main(){
	Calc calc;
	calc.Help();
	while(true){
		cout << ">";
		string s;
		getline(cin, s);
		
		if (s == "quit")
			break;

		calc.ProcessCommand(s);
	}
}