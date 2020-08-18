#pragma once

#include "common.h"
#include "Scanner.h"
#include "AST.h"
#include "VM.h"

class Parser
{
private:
	Scanner* scanner;
	AST* ast;

	// specific parsers
	InterpreterResults statement();
	InterpreterResults expression();


public:
	Parser(Scanner*);
	InterpreterResults parse();
};

