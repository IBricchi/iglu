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
	//information for expression
	TokenType delimiter;
	int unclosedParen;
	bool hadAssignment;
	bool allowAssignment;
	enum struct ExprType{
		DEF,
		ASSIGN
	};
	ExprType exprType;

	// states
	enum struct State {
		START, END,

		LEFT_PARAN, RIGHT_PARAN,
		CONST, UNARY, IDENT, OPERATOR,

		DOT_OP, ASSIGNMENT,

		FN_CALL, END_FN_CALL,
		PARAM, PARAM_COMMA,

		ERROR, PANIC_ERROR,
	};
	State lne;
	State state;

	static const std::unordered_map<State, std::vector<std::pair<TokenType, State>>> validTypeMap;
public:
	std::string errorMessage;

	PSM();
	
	void next(TokenType);
	bool is_error();
	bool is_panic();
	bool at_end();

	void setAsAssignment();

	void change_delimiter(TokenType);
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

