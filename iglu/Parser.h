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
	std::queue<Token> rpn;

	const Token utility_popToken = Token{TokenType::POP_STACK, Presidence::NONE, true, nullptr, 0, 0};

	// errors
	struct PanicException: public std::exception {
		const char* what() const throw();
	};
	void consume(TokenType);
	void reachDelimiter(TokenType);
	void panicError(std::string);
	void nonPanicError(std::string);

	// statement controls
	void statement();

	// expression controls
	enum struct ExpressionType {
		NONE,
		VAR_DEC
	};

	void expression(TokenType, ExpressionType = ExpressionType::NONE);

	// helpers
	std::string getName(Token);
	std::string getName(TokenType);
	void tryExpression(TokenType, ExpressionType = ExpressionType::NONE);

public:
	bool hadError;
	Parser(Scanner*);
	std::queue<Token>* getRPN();
	void parse();
};

