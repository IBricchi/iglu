#include "Number.h"

#include "Bool.h"
#include "Error.h"
#include "Function.h"
#include "Str.h"

using namespace std;

Number::Number(float val) : Object{} {
	this->val = val;
	type = "Number";

	addProperty("__to_string__",	&toString);
	addProperty("__negate__",		&negate);

	addProperty("__plus__",			&add);
	addProperty("__minus__",		&sub);
	addProperty("__star__",			&mult);
	addProperty("__slash__",		&div);

	addProperty("__equal_equal__",	&equal);
	addProperty("__not_equal__",	&notEqual);
	addProperty("__greater__",		&greater);
	addProperty("__greater_equal__",&greaterEqual);
	addProperty("__less__",			&less);
	addProperty("__less_equal__",	&lessEqual);

}

Object* Number::toStringFn() {
	if (val == (int)val) return new Str(to_string((int)val));
	return new Str(to_string(val));
}
LinkedUnoFn Number::toString = LinkedUnoFn((Object::unoFn) &Number::toStringFn);
Object* Number::negateFn() {
	return new Number(-val);
}
LinkedUnoFn Number::negate = LinkedUnoFn((Object::unoFn) &Number::negateFn);

Object* Number::addFn(Object* b) {
	if(b->checkType("Number") != 0) return new Error("Number does not support adding wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val + bVal);
}
LinkedBinFn Number::add = LinkedBinFn((Object::binFn) &Number::addFn);
Object* Number::subFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support being subtracted by " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val - bVal);
}
LinkedBinFn Number::sub = LinkedBinFn((Object::binFn) & Number::subFn);
Object* Number::multFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support multiplying by " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val * bVal);
}
LinkedBinFn Number::mult = LinkedBinFn((Object::binFn) & Number::multFn);
Object* Number::divFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support dividing by  " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val / bVal);
}
LinkedBinFn Number::div = LinkedBinFn((Object::binFn) & Number::divFn);

Object* Number::equalFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val == bVal);
}
LinkedBinFn Number::equal = LinkedBinFn((Object::binFn) &Number::equalFn);
Object* Number::notEqualFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val != bVal);
}
LinkedBinFn Number::notEqual = LinkedBinFn((Object::binFn) & Number::notEqualFn);
Object* Number::greaterFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val > bVal);
}
LinkedBinFn Number::greater = LinkedBinFn((Object::binFn) & Number::greaterFn);
Object* Number::greaterEqualFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val >= bVal);
}
LinkedBinFn Number::greaterEqual = LinkedBinFn((Object::binFn) & Number::greaterEqualFn);
Object* Number::lessFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val < bVal);
}
LinkedBinFn Number::less = LinkedBinFn((Object::binFn) & Number::lessFn);
Object* Number::lessEqualFn(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val <= bVal);
}
LinkedBinFn Number::lessEqual = LinkedBinFn((Object::binFn) & Number::lessEqualFn);

float Number::getVal() {
	return val;
}