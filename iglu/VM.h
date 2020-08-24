#pragma once

#include <vector>
#include <string>

#include "common.h"

#include "Chunk.h"
#include "Object.h"
#include "Constant.h"

#define FRAME_MAX 64;
#define STACK_MAX (FRAME_MAX * (UINT8_MAX + 1));

class VM
{
private:
	std::vector<Chunk*> chunks;
	std::vector<Object*> stack;
	std::vector<uint8_t*> pc;

	std::unordered_map<std::string, std::vector<Object*>> variables;
	
	//helpers
	uint8_t readByte();
	Constant readConstant();
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

