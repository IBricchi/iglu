#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Bool : public Object
{
private:
	bool val;

	Object* toString();
	Object* _not();

	Object* equal(Object*);
	Object* notEqual(Object*);

public:
	Bool(bool);

	bool getVal();
};