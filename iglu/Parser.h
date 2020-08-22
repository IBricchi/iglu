#pragma once

#include <queue>
#include <string>
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
	void consume(TokenType);
	void reachDelimiter(TokenType);
	void panicError(std::string);
	void nonPanicError(std::string);

	// specific parsers
	void statement();
	void expression(TokenType);

	// helpers
	std::string getName(Token);
	std::string getName(TokenType);
	void tryExpression(TokenType);

public:
	bool hadError;
	Parser(Scanner*);
	std::queue<Token>* getRPN();
	void parse();
};

