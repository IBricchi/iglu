#pragma once

#include <stack>

#include "common.h"
#include "Chunk.h"
#include "Object.h"

#define FRAME_MAX 64;
#define STACK_MAX (FRAME_MAX * (UINT8_MAX + 1));

enum struct InterpreterResults {
	OK,
	COMP_ERR,
	RUN_ERR
};

class VM
{
private:
	std::stack<Chunk*> chunks;
	std::stack<uint8_t*> pc;
	std::stack<Object*> objStack;
	
	//helpers
	uint8_t readByte();
	Object* readObject();
	Object* popStack();
	void pushStack(Object*);
	void intoChunk(Chunk*);
	void leaveChunk();

	//important
	InterpreterResults run();
public:
	VM();
	InterpreterResults interpret(Chunk*);
};

