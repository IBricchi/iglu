#pragma once

#include <string>

enum struct TokenType {
	LEFT_PARAN, RIGHT_PARAN,
	LEFT_BRACE, RIGHT_BRACE,
	PLUS, MINUS, STAR, SLASH,
	COMMA, DOT, SEMICOLON,
	ARROW,

	EQUAL, EQUAL_EQUAL,
	BANG, BANG_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	IDENTIFIER, STRING, NUMBER,

	AND, OR, IF, ELSE, RETURN,
	TRUE, FALSE, FOR, WHILE,
	LET, FUN, NIL,
	THIS, PARENT, CHILD_OF,

	ERROR,
	FILE_END
};

struct Token {
	TokenType type;
	string::const_iterator start;
	int length;
	int line;
};