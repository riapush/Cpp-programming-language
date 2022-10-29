#pragma once

#include<iostream>
#include<map>
#include<functional>
#include <iostream>
#include <stack>
#include <cctype>
#include <map>
#include <algorithm>
#include <string>
#include<cmath>

using mapOperators = std::map<std::string, std::function<double(double, double)>>;

using PriorityMap = std::map<char, size_t>;

class Operations {
	Operations();

	Operations(const Operations&) = delete;

	Operations& operator=(Operations const&) = delete;

	PriorityMap operations_priority;

	mapOperators operations;
public:

	static Operations& getOperations();

	int const priority(char const& operationName);

	double operation(double const& a, double const& b, std::string const& name);
};
