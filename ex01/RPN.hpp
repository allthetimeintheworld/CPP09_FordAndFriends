#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <stack>

class RPN
{
private:
	std::stack<int> _stack;
	
	bool isOperator(const std::string& token) const;
	bool isNumber(const std::string& token) const;
	void performOperation(const std::string& op);

public:
	// Orthodox Canonical Form
	RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);
	~RPN();

	// Methods
	int evaluate(const std::string& expression);
};

#endif
