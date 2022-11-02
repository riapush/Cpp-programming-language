#include "Calc.h"

int main()
{
    std::cout << "Available operations: +, -, *, /, ^";
	Calculator calc;
	std::string expression;
	std::cout << "Enter expression(enter z to escape):" << std::endl;
	std::getline(std::cin, expression);
	while (expression != "z") {
		try {
			std::cout << expression << "=" << calc.calculate(expression) << std::endl;
		}
		catch (std::exception& error) {
			std::cout << error.what() << std::endl;
		}
		std::cout << "Enter expression(enter z to escape):" << std::endl;
		std::getline(std::cin, expression);
	}

	return 0;
}