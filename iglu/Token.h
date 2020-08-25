#pragma once

#include <string>

enum struct TokenType {
	LEFT_PARAN, RIGHT_PARAN,
	LEFT_BRACE, RIGHT_BRACE,
	PLUS, MINUS, STAR, SLASH,
	COMMA, DOT, SEMICOLON,
	ARROW, NEGATE,

	EQUAL, EQUAL_EQUAL,
	BANG, BANG_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	IDENTIFIER, STRING, NUMBER,

	AND, OR, IF, ELSE, RETURN,
	TRUE, FALSE, FOR, WHILE,
	LET, FUN, NILL,
	THIS, PARENT, CHILD_OF,

	ERROR,
	FILE_END,

	// utilities
	POP_STACK
};

enum struct Presidence {
	NONE,		// none
	ASSIGNMENT,	// =
	OR,			// or
	AND,		// and
	EQUALITY,	// == !=
	COMPARISON,	// < > <= >=
	TERM,		// + -
	FACTOR,		// * /
	UNARY,		// ! -
	CALL,		// . ()
	PRIMARY,	// numbers strings identifiers
};

struct Token {
	TokenType type;
	Presidence presidence;
	bool leftAssoc;
	const char* start;
	int length;
	int line;

	static std::string tokenName(TokenType);
};