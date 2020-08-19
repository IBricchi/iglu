#pragma once

#include <queue>

#include "common.h"

#include "Chunk.h"
#include "Token.h"
#include "VM.h"

class Compiler
{
private:
	std::queue<Token>* rpn;
	Chunk* chunk;

// helpers
	Token popRpn();
	void tokenToChunk(Token);

// token specifics
	InterpreterResults statement();
	InterpreterResults expression();

public:
	Compiler();
	Chunk* getChunk();
	InterpreterResults compile(std::queue<Token>*);
};

