#pragma once

#include <queue>
#include <string>
#include <exception>

#include "common.h"

#include "Scanner.h"
#include "Token.h"
#include "VM.h"

class PSM
{
private:
	enum State {
		// overal states values
		BEGIN, STATEMENT, EXPRESSION,
		
		// error states
		PANIC, NON_PANIC
	};
	State state{BEGIN};
public:
	bool next(Token);
	bool is_panic();
	void reset();
};

class Parser
{
private:
	Scanner* scanner;
	Token current;
	std::queue<Token> rpn;
	PSM psm;

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

	void expression(TokenType);

	// helpers
	void tryStatement();
	void tryExpression(TokenType);

public:
	bool hadError;
	Parser(Scanner*);
	std::queue<Token>* getRPN();
	void parse();

	// helpers
	static std::string getName(Token);
	static std::string getName(TokenType);
};

