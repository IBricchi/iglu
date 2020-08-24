#pragma once

#include <string>

#include "common.h"

#include "Token.h"

class Scanner
{
private:
	// variables
	const char* start;
	const char* current;
	int line;
	Token prev;

	// controls
	char peek();
	char peekNext();
	char advance();
	bool match(char);

	// helpers
	bool isDigit(char);
	bool isAlpha(char);
	bool isAlphaNumeric(char);
	bool checkKeyword(int, int, const char*);
	TokenType identifierType();
	static Presidence getPresidence(TokenType);
	static bool isLeftAssoc(TokenType);

	// token specifics
	void skipWhiteSpace();
	Token makeToken(TokenType);
	Token errorToken(const std::string&);
	Token stringToken();
	Token numberToken();
	Token identifierToken();
public:
	Scanner(const char* source);
	bool isAtEnd();
	Token scanToken();
};

