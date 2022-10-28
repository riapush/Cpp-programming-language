#pragma once
#include <iostream>
#include <stack>
#include <cctype>
#include <map>
#include <algorithm>
#include <string>
#include<cmath>

class Calculator {
	std::stack<double> numbers;
	std::string postfix(std::string exp);
	void add_plugins();
public:
	Calculator();
	~Calculator();
	double calculate(std::string exp);
};


