#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "common.h"

#include "Chunk.h"
#include "Object.h"
#include "Constant.h"

#define FRAME_MAX 64;
#define STACK_MAX (FRAME_MAX * (UINT8_MAX + 1));

struct RefObj {
	std::string* ref;
	Object* obj;
};

class VM
{
private:
	std::vector<Chunk*> chunks;
	std::vector<RefObj> stack;
	std::vector<uint8_t*> pc;
	std::unordered_set<Object*> objs;

	std::unordered_map<std::string, std::vector<Object*>> variables;
	
	//helpers
	uint8_t readByte();
	Constant readConstant();

	std::string popStackRef();
	Object* popStackObj();

	std::string topStackRef();
	Object* topStackObj();
	
	std::string stackAtRef(int);
	Object* stackAtObj(int);

	void pushStack(Object*);
	void pushStack(std::string*);

	void intoChunk(Chunk*);
	void leaveChunk();

	bool callFunction(Object*, std::string);
	void tryCallFunction(Object*, std::string, std::string);

	void runtimeError(std::string);
	void runtimeErrorObject(Object*);

	void cleanGarbage();

	//important
	void run();
public:
	bool hadError;

	VM();
	void interpret(Chunk*);
};

