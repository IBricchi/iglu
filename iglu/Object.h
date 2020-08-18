#pragma once

#include <map>
#include <vector>
#include <string>

#include "common.h"
#include "Chunk.h"
#include "VM.h"

class Object
{
protected:
	void* val;
public:
	std::map<std::string, Chunk*> properties;
	std::vector<Object* (*)(Object*, Object*)> binaryFuncs;
	Object();
	~Object();
	void* getVal();
};

