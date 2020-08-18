#pragma once

#include <map>
#include <vector>
#include <string>

#include "common.h"
#include "Chunk.h"
#include "VM.h"

class Object
{
private:
	std::map<std::string, Chunk*> properties;
public:
	Object(){};
	void* val;
	~Object();
	Chunk* plus(VM*, Object*);
};

