#include "Str.h"

#include "Bool.h"
#include "Error.h"

using namespace std;

Str::Str(string val) : Object{} {
	this->val = val;
	type = "String";

	addFnProperty("__plus__", (Object::binFn) & Str::concat);
	addFnProperty("__equal_equal__", (Object::binFn) & Str::compare);
	addFnProperty("__not_equal__", (Object::binFn) & Str::notCompare);
}

Str::Str(string* val) : Object{} {
	this->val = *val;
	type = "String";

	addFnProperty("__plus__", (Object::binFn) &Str::concat);
	addFnProperty("__equal_equal__", (Object::binFn) & Str::compare);
	addFnProperty("__not_equal__", (Object::binFn) & Str::notCompare);
}

Object* Str::concat(Object* b) {
	string bVal = b->toString();
	return new Str(val + bVal);
}

Object* Str::compare(Object* b) {
	if (b->checkType("String") != 0) return new Error("String does not support comparing wtih " + b->getType() + ".");
	string bVal = b->toString();
	return new Bool(val == bVal);
}

Object* Str::notCompare(Object* b) {
	if (b->checkType("String") != 0) return new Error("String does not support comparing wtih " + b->getType() + ".");
	string bVal = b->toString();
	return new Bool(val != bVal);
}

string Str::toString(){
	return val;
}

string Str::getVal() {
	return val;
}