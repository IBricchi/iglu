#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <string>

#include "common.h"

#include "Chunk.h"

class Object
{
public:
	typedef Object* (Object::*unoFn)();
	typedef Object* (Object::*binFn)(Object*);
protected:
	std::string type;
	std::queue<std::string> currentReference;

	// cpp to iglu function linking
	Chunk* generateUnoFnChunk(int);
	Chunk* generateBinFnChunk(int);
	void addFnProperty(std::string, Object::unoFn);
	void addFnProperty(std::string, Object::binFn);

	// cpp to iglu function linking
	void addProperty(std::string, Object*);
	
	// cpp functions to link to iglu
	Object* toStringFn();
	static LinkedUnoFn toString;
public:
	std::unordered_map<std::string, Object*> properties;
	//std::vector<Object::unoFn> unoFns;
	//std::vector<Object::binFn> binFns;

	Object();
	Object(bool);

	std::string getType();
	int checkType(std::string);

	void reference(std::string);
	std::string dereference();

	// debugging shit
	std::string virtual debugToString();
};
