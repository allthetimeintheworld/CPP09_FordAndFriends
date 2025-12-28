#include "RPN.hpp"
#include <iostream>
#include <exception>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: invalid number of arguments" << std::endl;
		std::cerr << "Usage: ./RPN \"expression\"" << std::endl;
		return 1;
	}

	try
	{
		RPN calculator;
		int result = calculator.evaluate(argv[1]);
		std::cout << result << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
