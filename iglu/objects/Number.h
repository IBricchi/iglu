#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Number : public Object
{
private:
	float val;

	Object* toStringFn();
	static LinkedUnoFn toString;
	Object* negateFn();
	static LinkedUnoFn negate;

	Object* addFn(Object*);
	static LinkedBinFn add;
	Object* subFn(Object*);
	static LinkedBinFn sub;
	Object* multFn(Object*);
	static LinkedBinFn mult;
	Object* divFn(Object*);
	static LinkedBinFn div;

	Object* equalFn(Object*);
	static LinkedBinFn equal;
	Object* notEqualFn(Object*);
	static LinkedBinFn notEqual;
	Object* greaterFn(Object*);
	static LinkedBinFn greater;
	Object* greaterEqualFn(Object*);
	static LinkedBinFn greaterEqual;
	Object* lessFn(Object*);
	static LinkedBinFn less;
	Object* lessEqualFn(Object*);
	static LinkedBinFn lessEqual;
public:
	Number(float);

	float getVal();

	// debugging shit
	std::string debugToString() override;
};