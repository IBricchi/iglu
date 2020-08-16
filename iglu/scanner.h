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
	int col;
	int line;

	// controls
	bool isAtEnd();
	char peek();
	char peekNext();
	char advance();

	// helpers

	// token specifics
	void skipWhiteSpace();
public:
	scanner(const string& source);
	Token scanToken();
};

