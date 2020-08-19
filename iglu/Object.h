#pragma once

#include <map>
#include <vector>
#include <string>

#include "common.h"

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

