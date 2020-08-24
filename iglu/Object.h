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
	std::unordered_multiset<std::string> references;
	std::queue<std::string> currentReference;

	// cpp to iglu function linking
	Chunk* generateFnChunk(Object::unoFn);
	Chunk* generateFnChunk(Object::binFn);
	void addFnProperty(std::string, Object::unoFn);
	void addFnProperty(std::string, Object::binFn);
public:
	std::unordered_map<std::string, Chunk*> properties;
	std::vector<Object::unoFn> unoFns;
	std::vector<Object::binFn> binFns;

	Object();
	~Object();

	std::string getType();
	int checkType(std::string);

	void reference(std::string);
	std::string dereference();
	std::string getReference();

	void addReference(std::string);
	void removeReference(std::string);
	void giveImortality();
	void removeImortality();
};
