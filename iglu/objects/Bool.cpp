#include "Bool.h"

#include "Number.h"
#include "Error.h"
#include "Function.h"
#include "Str.h"

using namespace std;

Bool::Bool(bool val) : Object{} {
	this->val = val;
	type = "Bool";

	addProperty("__to_string__", &toString);
	addProperty("__not__", &_not);

	addProperty("__equal_equal__", &equal);
	addProperty("__not_equal__", &notEqual);
}

Object* Bool::toStringFn() {
	return new Str(val ? "true" : "false");
}
LinkedUnoFn Bool::toString = LinkedUnoFn((Object::unoFn) &Bool::toStringFn);
Object* Bool::_notFn() {
	return new Bool(!val);
}
LinkedUnoFn Bool::_not = LinkedUnoFn((Object::unoFn) &Bool::_notFn);

Object* Bool::equalFn(Object* b) {
	if (b->checkType("Bool") != 0) return new Error("Bool does not support comparing wtih " + b->getType() + ".");
	bool bVal = ((Bool*)b)->getVal();
	return new Bool(val == bVal);
}
LinkedBinFn Bool::equal = LinkedBinFn((Object::binFn) &Bool::equalFn);

Object* Bool::notEqualFn(Object* b) {
	if (b->checkType("Bool") != 0) return new Error("Bool does not support comparing wtih " + b->getType() + ".");
	bool bVal = ((Bool*)b)->getVal();
	return new Bool(val != bVal);
}
LinkedBinFn Bool::notEqual = LinkedBinFn((Object::binFn) &Bool::notEqualFn);

bool Bool::getVal() {
	return val;
}