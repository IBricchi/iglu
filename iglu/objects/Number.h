#pragma once

#include "../common.h"

#include "../Object.h"

class Number : public Object
{
private:
	float val;

	Object* negate();
	Object* add(Object*);
	Object* sub(Object*);
	Object* mult(Object*);
	Object* div(Object*);
public:
	Number(float);

	float getVal();
};