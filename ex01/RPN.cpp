#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

// Orthodox Canonical Form
RPN::RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
		_stack = other._stack;
	return *this;
}

RPN::~RPN() {}

// Helper functions
bool RPN::isOperator(const std::string& token) const
{
	return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isNumber(const std::string& token) const
{
	if (token.empty())
		return false;
	
	// Check for negative numbers
	size_t start = 0;
	if (token[0] == '-' || token[0] == '+')
		start = 1;
	
	// Check if remaining characters are digits
	for (size_t i = start; i < token.length(); ++i)
	{
		if (!isdigit(token[i]))
			return false;
	}
	
	return token.length() > start; 
	// Make sure there's at least one digit
}

void RPN::performOperation(const std::string& op)
{
	if (_stack.size() < 2)
		throw std::runtime_error("Error: insufficient operands");
	
	// Pop two operands (note the order!) remember stack oder you dolt!
	int b = _stack.top();
	_stack.pop();
	int a = _stack.top();
	_stack.pop();
	
	int result;
	
	if (op == "+")
		result = a + b;
	else if (op == "-")
		result = a - b;
	else if (op == "*")
		result = a * b;
	else if (op == "/")
	{
		if (b == 0)
			throw std::runtime_error("Error: division by zero");
		result = a / b;
	}
	else
		throw std::runtime_error("Error: unknown operator");
	
	_stack.push(result);
}

int RPN::evaluate(const std::string& expression)
{
	std::istringstream iss(expression);
	std::string token;
	
	// Clear the stack
	while (!_stack.empty())
		_stack.pop();
	
	while (iss >> token)
	{
		if (isNumber(token))
		{
			int num = atoi(token.c_str());
			_stack.push(num);
		}
		else if (isOperator(token))
		{
			performOperation(token);
		}
		else
		{
			throw std::runtime_error("Error: invalid token");
		}
	}
	
	// After processing all tokens, stack should have exactly one value
	if (_stack.size() != 1)
		throw std::runtime_error("Error: invalid expression");
	
	return _stack.top();
}
