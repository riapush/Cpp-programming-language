#include "Calc.h"


std::string Calculator::charToStr(char c) {
	std::stringstream stringstream;
	std::string str;
	stringstream << c;
	stringstream >> str;
	return str;
}

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

	std::stack<std::string> stack;
	for (auto c : exp) {
		if (!isdigit(c) && ('.' != c)) {
			if (!isalpha(c)) {
				res += ' ';
			}
			else {
				res += c;
				continue;
			}
			if (')' == c) {
				while (stack.top() != "(") {
					res += stack.top();
					stack.pop();
					res += ' ';
				}
				stack.pop();
			}
			else if ('(' == c) {
				stack.push(charToStr(c));
			}
			else if (stack.empty() || ((Operations::getOperations().contains(charToStr(c))) && (Operations::getOperations().priority(stack.top()) < Operations::getOperations().priority(charToStr(c))))) {
				stack.push(charToStr(c));
			}
			else {
				do {
					res += stack.top();
					res += ' ';
					stack.pop();
				} while (!(stack.empty() || (Operations::getOperations().priority(stack.top()) < Operations::getOperations().priority(charToStr(c)))));
				stack.push(charToStr(c));
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

	return res;
}

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
			if (!(curr_num == "")) {
				double num = std::atof(curr_num.c_str());
				numbers.push(num);
				curr_num = "";
			}
			try {
                double a,b;
				if (Operations::getOperations().contains(charToStr(exp[i]))) {
                    a = numbers.top();
                    numbers.pop();
                    b = 0;
					b = numbers.top();
					numbers.pop();
					numbers.push(Operations::getOperations().operation(a, b, charToStr(exp[i])));
				}
				else {
					std::string cur_op = "";
					if (isalpha(exp[i])) {
						while (isalpha(exp[i])) {
							cur_op += exp[i];
							i++;
						}
					}
					if (pl.binary_contains(cur_op)) {
                        curr_num = "";
                        while (isdigit(exp[i]) || exp[i] == ' ' || exp[i] == '.') {
                            if(exp[i] == ' '){
                                continue;
                            }
                            curr_num += exp[i];
                            i++;
                        }
                        double num = std::atof(curr_num.c_str());
                        numbers.push(num);
                        curr_num = "";
                        a = numbers.top();
                        numbers.pop();
						b = numbers.top();
						numbers.pop();
						numbers.push(pl.operation(a, b, cur_op));
					}
					else if (pl.unary_contains(cur_op)) {
                        curr_num = "";
                        i++;
                        while (exp[i] != ' ') {
                            curr_num += exp[i];
                            i++;
                        }
                        double num = std::atof(curr_num.c_str());
                        numbers.push(num);
                        curr_num = "";
                        a = numbers.top();
                        numbers.pop();
                        b = 0;
						numbers.push(pl.operation(a, b, cur_op));
					}
					else {
						throw std::exception();
					}
				}
			}
			catch (std::exception& e) {
				std::cout << e.what();
			}
		}
	}
	return numbers.top();
}

Calculator::Calculator()=default;