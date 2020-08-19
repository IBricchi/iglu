#pragma once

#include <queue>

#include "common.h"

#include "Scanner.h"
#include "Token.h"
#include "VM.h"

class Parser
{
private:
	Scanner* scanner;
	std::queue<Token>* rpn;

	// specific parsers
	InterpreterResults statement();
	InterpreterResults expression();


public:
	Parser(Scanner*);
	InterpreterResults parseStatement();
};

