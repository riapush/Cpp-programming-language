#include "Calc.h"
#include "Operations.h"

std::string Calculator::postfix(std::string exp) {
	std::string res;

	std::string::size_type ind;
	while ((ind = exp.find(' ')) != std::string::npos) exp.erase(ind, 1); // delete spaces

	for (size_t i = 0; i < exp.size(); ++i) { // for negative numbers, we change (-num) to (0-num)
		if ((exp[i] == '+' || exp[i] == '-') && (0 == i || (!isdigit(exp[i - 1]) && exp[i - 1] != '.' && exp[i - 1] != ')'))) {
			auto it = std::find_if(exp.begin() + i + 1, exp.end(), [](char const c) {return !isdigit(c); });
			exp.insert(it, ')');
			exp.insert(i, "(0");
		}
	}

	std::cout << exp << std::endl;

	std::stack<char> stack;
	for (auto c : exp) {
		if (!isdigit(c) && ('.' != c)) {
			res += ' ';
			if (')' == c) {
				while (stack.top() != '(') {
					res += stack.top();
					stack.pop();
					res += ' ';
				}
				stack.pop();
			}
			else if ('(' == c) {
				stack.push(c);
			}
			else if (stack.empty() || (Operations::getOperations().priority(stack.top()) < Operations::getOperations().priority(c))) {
				stack.push(c);
			}
			else {
				do {
					res += stack.top();
					res += ' ';
					stack.pop();
				} while (!(stack.empty() || (Operations::getOperations().priority(stack.top()) < Operations::getOperations().priority(c))));
				stack.push(c);
			}
		}
		else {
			res += c;
		}
	}
	while (!stack.empty()) {
		res += stack.top();
		res += ' ';
		stack.pop();
	}
	std::cout << "Postfix res:" << res << std::endl; // result of reverse polish notation

	return res;
}

void Calculator::add_plugins() {
	// this func should add .dll but author of code kinda sucks in cpp
}
Calculator::~Calculator() = default;

double Calculator::calculate(std::string exp) {
	exp = postfix(exp);
	std::string curr_num = "";
	for (size_t i = 0; i < exp.size(); i++) {
		if (exp[i] == ' ') {
			if (curr_num == "") {
				continue;
			}
			double num = std::atof(curr_num.c_str());
			numbers.push(num);
			curr_num = "";
			continue;
		}
		if (isdigit(exp[i]) || exp[i] == '.') {
			curr_num.push_back(exp[i]);
		}
		else {
			if (!isdigit(exp[i]) && exp[i] != '^' && exp[i] != '-' && exp[i] != '+' && exp[i] != '*' && exp[i] != '/') {
				throw std::invalid_argument("received non-number argument or invalid operation");
			}
			if (!(curr_num == "")) {
				double num = std::atof(curr_num.c_str());
				numbers.push(num);
				curr_num = "";
			}
			try {
				double a = numbers.top();
				numbers.pop();
				double b = numbers.top();
				numbers.pop();
				switch (exp[i]) {
				case '+':
					numbers.push(b + a);
					break;
				case'-':
					numbers.push(b - a);
					break;
				case '*':
					numbers.push(b * a);
					break;
				case'/':
					numbers.push(b / a);
					break;
				case'^':
					numbers.push(pow(b, a));
				}
			}
			catch (std::exception& e) {
				e.what();
			}
		}
	}
	return numbers.top();
}

Calculator::Calculator() {
	add_plugins();
}