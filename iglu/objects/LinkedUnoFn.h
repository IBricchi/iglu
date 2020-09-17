#pragma once

#include "../common.h"

#include "../Object.h"
#include "Function.h"

#include "Str.h"

class LinkedUnoFn : public Function {
public:
	LinkedUnoFn(Object::unoFn);
	void setupLinks() override;
private:
	Object::unoFn fn;

	// static Linked Fn objects
	Object* toStringFn();
	static LinkedUnoFn toString;
};