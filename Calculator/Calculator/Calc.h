#pragma once
#include <iostream>
#include <stack>
#include <cctype>
#include <map>
#include <algorithm>
#include <string>
#include<cmath>
#include <sstream>
#include "Operations.h"
#include "PluginLoader.h"

class Calculator {
	std::stack<double> numbers;
	std::string postfix(std::string exp);
	std::string charToStr(char c);
    PluginLoader pl;

public:
	Calculator();
	~Calculator()=default;
	double calculate(std::string exp);
};


