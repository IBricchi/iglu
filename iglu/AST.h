#pragma once

#include "Token.h"

struct AST
{
	Token token;
	AST* parent;
	AST* left;
	AST* right;
	AST(AST*, Token);
	~AST();
	AST* push_left(Token);
	AST* push_right(Token);
};

