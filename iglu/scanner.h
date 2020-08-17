#pragma once

#include <string>

#include "token.h"

using namespace std;

class Scanner
{
private:
	// variables
	string::const_iterator start;
	string::const_iterator current;
	string::const_iterator end;
	int line;

	// controls
	char peek();
	char peekNext();
	char advance();
	char isAlpha();

	// helpers

	// token specifics
	void skipWhiteSpace();
	Token makeToken(TokenType);
	Token errorToken(const string&);
	Token stringToken();
public:
	Scanner(const string&);
	bool isAtEnd();
	Token scanToken();
};

