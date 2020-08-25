#include "Bool.h"

#include "Number.h"
#include "Error.h"

using namespace std;

Bool::Bool(bool val) : Object{} {
	this->val = val;
	type = "Bool";

	addFnProperty("__not__", (Object::unoFn) &Bool::_not);
	addFnProperty("__equal_equal__", (Object::binFn) &Bool::equal);
	addFnProperty("__not_equal__", (Object::binFn) &Bool::notEqual);
}

Object* Bool::_not() {
	return new Bool(!val);
}

Object* Bool::equal(Object* b) {
	if (b->checkType("Bool") != 0) return new Error("Bool does not support comparing wtih " + b->getType() + ".");
	bool bVal = ((Bool*)b)->getVal();
	return new Bool(val == bVal);
}

Object* Bool::notEqual(Object* b) {
	if (b->checkType("Bool") != 0) return new Error("Bool does not support comparing wtih " + b->getType() + ".");
	bool bVal = ((Bool*)b)->getVal();
	return new Bool(val != bVal);
}

string Bool::toString() {
	return val?"true":"false";
}

bool Bool::getVal() {
	return val;
}