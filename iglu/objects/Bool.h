#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Bool : public Object
{
private:
	bool val;

	Object* toStringFn();
	static LinkedUnoFn toString;
	Object* _notFn();
	static LinkedUnoFn _not;

	Object* equalFn(Object*);
	static LinkedBinFn equal;
	Object* notEqualFn(Object*);
	static LinkedBinFn notEqual;

public:
	Bool(bool);

	bool getVal();
};