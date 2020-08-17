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

	// controls
	char peek();
	char peekNext();
	char advance();

	// helpers
	bool isDigit(char);
	bool isAlpha(char);
	bool isAlphaNumeric(char);
	bool checkKeyword(int, int, const char*);
	TokenType identifierType();

	// token specifics
	void skipWhiteSpace();
	Token makeToken(TokenType);
	Token errorToken(const string&);
	Token stringToken();
	Token numberToken();
	Token identifierToken();
public:
	Scanner(const string& source);
	bool isAtEnd();
	Token scanToken();
};

