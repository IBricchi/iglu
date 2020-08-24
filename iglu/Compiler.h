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
	Chunk chunk;

// helpers
	Token popRpn();
	Token frontRpn();
	void tokenToChunk(Token);

// token specifics
	void statement();
	void expression();

public:
	Compiler();
	Chunk* getChunk();
	void compile(std::queue<Token>*);
};
