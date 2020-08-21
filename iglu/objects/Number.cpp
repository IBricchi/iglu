#include "Number.h"
#include "Error.h"

using namespace std;

Number::Number(float val) : Object{} {
	this->val = val;
	type = "Number";

	addFnProperty("__negate__", (Object::unoFn) &Number::negate);

	addFnProperty("__plus__", (Object::binFn) &Number::add);
	addFnProperty("__minus__", (Object::binFn) &Number::sub);
	addFnProperty("__star__", (Object::binFn) &Number::mult);
	addFnProperty("__slash__", (Object::binFn) &Number::div);
}

Object* Number::negate() {
	return new Number(-val);
}

Object* Number::add(Object* b) {
	if(!b->checkType("Number") == 0) return new Error("Number does not support adding wtih " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val + bVal);
}

Object* Number::sub(Object* b) {
	if (!b->checkType("Number") == 0) return new Error("Number does not support being subtracted by " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val - bVal);
}

Object* Number::mult(Object* b) {
	if (!b->checkType("Number") == 0) return new Error("Number does not support multiplying by " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val * bVal);
}

Object* Number::div(Object* b) {
	if (!b->checkType("Number") == 0) return new Error("Number does not support dividing by  " + b->getType() + ".");
	float bVal = ((Number*)b)->getVal();
	return new Number(val / bVal);
}

float Number::getVal() {
	return val;
}