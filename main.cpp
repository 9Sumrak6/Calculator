#include <iostream>
#include <string>
#include "Calculator.hpp"

using namespace std;

int main(){
	Calc calc;
	while(1){
		cout << ">";
		string s;
		getline(cin, s);
		
		if (s == "quit")
			break;

		calc.ProcessCommand(s);
	}
}