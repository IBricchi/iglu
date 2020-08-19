#pragma once

#include <stack>
#include <string>

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
	std::stack<Object*> objStack;
	std::stack<uint8_t*> pc;
	
	//helpers
	uint8_t readByte();
	Object* readObject();
	Object* popStack();
	Object* topStack();
	void pushStack(Object*);
	void intoChunk(Chunk*);
	void leaveChunk();
	bool callFunction(Object*, std::string);
	void runtimeError(std::string);

	//important
	InterpreterResults run();
public:
	VM();
	InterpreterResults interpret(Chunk*);
};

