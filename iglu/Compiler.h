#pragma once

#include "VM.h"
#include "Scanner.h"
#include "Token.h"

class compiler
{
private:
	Scanner scanner;
	VM vm;
	Token current;
	Token previous;

	// controlls
	void advance();
	void consume(TokenType, const char*);

	// error messages
	void errorAt(Token&, const char*);
	void error(const char*);
	void errorAtCurrent(const char *);

public:
	
};

