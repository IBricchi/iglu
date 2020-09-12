#include "Number.h"

#include "Bool.h"
#include "Error.h"
#include "Str.h"

using namespace std;

Number::Number(float val) : Object{} {
	this->val = val;
	type = "Number";

	addFnProperty("__to_string__",	(Object::unoFn) &Number::toString);
	addFnProperty("__negate__",		(Object::unoFn) &Number::negate);

	addFnProperty("__plus__",		(Object::binFn) &Number::add);
	addFnProperty("__minus__",		(Object::binFn) &Number::sub);
	addFnProperty("__star__",		(Object::binFn) &Number::mult);
	addFnProperty("__slash__",		(Object::binFn) &Number::div);

	addFnProperty("__equal_equal__",	(Object::binFn) & Number::equal);
	addFnProperty("__not_equal__",		(Object::binFn) & Number::notEqual);
	addFnProperty("__greater__",			(Object::binFn) & Number::greater);
	addFnProperty("__greater_equal__",		(Object::binFn) & Number::greaterEqual);
	addFnProperty("__less__",		(Object::binFn) & Number::less);
	addFnProperty("__less_equal__",	(Object::binFn) & Number::lessEqual);

}

Object* Number::toString() {
	if (val == (int)val) return new Str(to_string((int)val));
	return new Str(to_string(val));
}
Object* Number::negate() {
	return new Number(-val);
}

Object* Number::add(Object* b) {
	if(b->checkType("Number") != 0) return new Error("Number does not support adding wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val + bVal);
}
Object* Number::sub(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support being subtracted by " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val - bVal);
}
Object* Number::mult(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support multiplying by " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val * bVal);
}
Object* Number::div(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support dividing by  " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val / bVal);
}

Object* Number::equal(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val == bVal);
}
Object* Number::notEqual(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val != bVal);
}
Object* Number::greater(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val > bVal);
}
Object* Number::greaterEqual(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val >= bVal);
}
Object* Number::less(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val < bVal);
}
Object* Number::lessEqual(Object* b) {
	if (b->checkType("Number") != 0) return new Error("Number does not support comparing values wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Bool(val <= bVal);
}

float Number::getVal() {
	return val;
}