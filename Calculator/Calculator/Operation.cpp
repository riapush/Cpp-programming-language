#include "Operations.h"

Operations::Operations() {
	operations["+"] = [](double a, double b)->double {return b + a; };
	operations["-"] = [](double a, double b)->double {return b - a; };
	operations["*"] = [](double a, double b)->double {return b * a; };
	operations["/"] = [](double a, double b)->double {return b / a; };
	operations["--"] = [](double a, double b)->double {return 0 - a; };
	operations["^"] = [](double a, double b) {return pow(b, a); };

	operations_priority.insert(std::make_pair("^", 4));
	operations_priority.insert(std::make_pair("*", 3));
	operations_priority.insert(std::make_pair("/", 3));
	operations_priority.insert(std::make_pair("+", 2));
	operations_priority.insert(std::make_pair("-", 2));
	operations_priority.insert(std::make_pair("(", 1));
};

Operations& Operations::getOperations() {
	static Operations instance;
	return instance;
};

int const Operations::priority(std::string const& operation) {
	return operations_priority[operation];
}

double Operations::operation(double const& a, double const& b, std::string const& name) {
	return operations[name](a, b);
};

bool Operations::contains(std::string const& name) {
	return operations.contains(name);
}