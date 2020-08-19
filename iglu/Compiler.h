#pragma once

#include "common.h"

#include "Scanner.h"
#include "Token.h"

class Compiler
{
private:
	Parser* parser;

public:
	Compiler(Parser*);
};

