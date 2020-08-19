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
	typedef Object* (Object::*binFn)(Object*);
	std::map<std::string, Chunk*> properties;
	std::vector<Object::binFn> binFns;
	Object();
	~Object();
	void* getVal();
};

