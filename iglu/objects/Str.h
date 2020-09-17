#pragma once

#include <string>

#include "../common.h"

#include "../Object.h"

class Str : public Object
{
private:
	std::string val;

	Object* toStringFn();
	static LinkedUnoFn toString;

	Object* concatFn(Object*);
	static LinkedBinFn concat;
	Object* compareFn(Object*);
	static LinkedBinFn compare;
	Object* notCompareFn(Object*);
	static LinkedBinFn notCompare;
public:
	Str(std::string);
	Str(std::string*);

	std::string getVal();

	// debugging shit
	std::string debugToString() override;
};