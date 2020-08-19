#pragma once

#include "../common.h"

#include "../Chunk.h"
#include "../Object.h"

class Number : public Object
{
private:
	Object* add(Object*);
public:
	Number(float);
};