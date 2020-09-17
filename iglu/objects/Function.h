#pragma once

#include <vector>

#include "../common.h"

#include "../Object.h"

class Function : public Object
{
protected:
	Function();
	Function(bool);

	int airity;
	Chunk* chunk;

	Object* toStringFn();
	static LinkedUnoFn toString;
	
	Object* callFn();
	static LinkedUnoFn call;

	//list of all linked functions
	static std::vector<Function*> linkedFns;
public:
	Function(Chunk*);
	~Function();

	Chunk* getChunk();
	static void linkFns();
	void virtual setupLinks();
};

#include "LinkedUnoFn.h"
#include "LinkedBinFn.h"