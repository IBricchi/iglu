#pragma once

#include <map>
#include <vector>
#include <string>

#include "common.h"

#include "Chunk.h"

class Object
{
public:
	typedef Object* (Object::*unoFn)();
	typedef Object* (Object::*binFn)(Object*);
protected:
	void* val;
	Chunk* generateFnChunk(Object::unoFn);
	Chunk* generateFnChunk(Object::binFn);
	void addFnProperty(std::string, Object::unoFn);
	void addFnProperty(std::string, Object::binFn);
public:
	std::string type;
	std::map<std::string, Chunk*> properties;
	std::vector<Object::unoFn> unoFns;
	std::vector<Object::binFn> binFns;
	int checkType(std::string);
	Object();
	~Object();
	void* getVal();
};

