#include <string>

#include "token.h"

using namespace std;

#pragma once
class scanner
{
private:
	// variables
	string::const_iterator start;
	string::const_iterator current;
	string::const_iterator end;
	int line;

	// controls
	bool isAtEnd();
	char peek();
	char peekNext();
	char advance();

	// helpers

	// token specifics
	Token makeToken(TokenType);
	Token errorToken(const string&);
	void skipWhiteSpace();
public:
	scanner(const string&);
	Token scanToken();
};

