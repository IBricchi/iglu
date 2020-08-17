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

	// helpers

	// token specifics
	Token makeToken(TokenType);
	Token errorToken(const string&);
	void skipWhiteSpace();
public:
	Scanner(const string&);
	bool isAtEnd();
	Token scanToken();
};

