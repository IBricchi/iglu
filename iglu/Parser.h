#pragma once

#include <queue>
#include <exception>

#include "common.h"

#include "Scanner.h"
#include "Token.h"
#include "VM.h"

class Parser
{
private:
	Scanner* scanner;
	Token current;
	std::queue<Token>* rpn;

	// errors
	struct PanicException: public std::exception {
		const char* what() const throw();
	};
	void PanicError(std::string);

	// specific parsers
	bool statement();
	bool expression(TokenType);


public:
	Parser(Scanner*);
	std::queue<Token>* getRPN();
	bool parse();
};

