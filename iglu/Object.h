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
	std::string type; // object type
	std::queue<std::string> currentReference; // references used to understand context of object use
	bool marked; // mark for garbage collection

	// cpp to iglu function linking
	Chunk* generateUnoFnChunk(int);
	Chunk* generateBinFnChunk(int);
	void addFnProperty(std::string, Object::unoFn);
	void addFnProperty(std::string, Object::binFn);

	// add string -> object relationship (basically variables)
	void addProperty(std::string, Object*);
	
	// cpp functions to link to iglu
	Object* toStringFn();
	static LinkedUnoFn toString;
public:
	std::unordered_map<std::string, Object*> properties; // map of variables -> objects

	// object constructors
	Object();
	Object(bool);

	// type checking functions
	std::string getType();
	int checkType(std::string);

	// referencing (I'd like to remove this, but at the moment it's required for variables, and will be used for function calls)
	void reference(std::string);
	std::string dereference();

	// garbage collection
	void mark();
	bool checkMark();
	void unmark();

	// debugging shit
	std::string virtual debugToString();
};
