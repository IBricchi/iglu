#pragma once

#include "../Object.h"

class Number : public Object
{
private:
	static Object* add(Object*, Object*);
public:
	Number(float);
};