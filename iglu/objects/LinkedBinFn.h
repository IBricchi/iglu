#pragma once

#include "../common.h"

#include "../Object.h"
#include "Function.h"

#include "Str.h"

class LinkedBinFn : public Function {
public:
	LinkedBinFn(Object::binFn);
	void setupLinks() override;
	Object* callFn(Object*, Object*);
private:
	Object::binFn fn;

	// static Linked Fn objects
	Object* toStringFn();
	static LinkedUnoFn toString;
};