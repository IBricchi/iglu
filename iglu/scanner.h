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
	bool isDigit(char);
	bool isAlpha(char);

	// helpers

	// token specifics
	void skipWhiteSpace();
	Token makeToken(TokenType);
	Token errorToken(const string&);
	Token stringToken();
	Token numberToken();
public:
	Scanner(const string& source);
	bool isAtEnd();
	Token scanToken();
};

