#pragma once

#include <stack>
#include <string>

#include "common.h"

#include "Chunk.h"
#include "Object.h"

#define FRAME_MAX 64;
#define STACK_MAX (FRAME_MAX * (UINT8_MAX + 1));

class VM
{
private:
	std::stack<Chunk*> chunks;
	std::stack<Object*> objStack;
	std::stack<uint8_t*> pc;
	//std::vector<std::pair<std::string, Object*>> variables;
	
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
	void runtimeErrorObject(Object*);

	//important
	void run();
public:
	bool hadError;

	VM();
	void interpret(Chunk*);
};

