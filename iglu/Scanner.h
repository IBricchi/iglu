#pragma once

#include <string>

#include "token.h"

using namespace std;

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
	bool isAtEnd();
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
	Token errorToken(const string&);
	Token stringToken();
	Token numberToken();
	Token identifierToken();
public:
	Scanner(const char* source);
	Token scanToken();
};

