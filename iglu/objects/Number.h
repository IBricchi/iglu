#pragma once

#include "../Object.h"

class Number : public Object
{
private:
	Object* add(Object*);
public:
	Number(float);
};