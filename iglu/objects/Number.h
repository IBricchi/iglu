#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Number : public Object
{
private:
	float val;

	Object* toString();
	Object* negate();

	Object* add(Object*);
	Object* sub(Object*);
	Object* mult(Object*);
	Object* div(Object*);

	Object* equal(Object*);
	Object* notEqual(Object*);
	Object* greater(Object*);
	Object* greaterEqual(Object*);
	Object* less(Object*);
	Object* lessEqual(Object*);
public:
	Number(float);

	float getVal();
};