#include "Number.h"

using namespace std;

Number::Number(float val) : Object{} {
	this->val = new float(val);

	addFnProperty("__negate__", (Object::unoFn) &Number::negate);

	addFnProperty("__plus__", (Object::binFn) &Number::add);
	addFnProperty("__minus__", (Object::binFn) &Number::sub);
	addFnProperty("__star__", (Object::binFn) &Number::mult);
	addFnProperty("__slash__", (Object::binFn) &Number::div);
}

Object* Number::negate() {
	float aVal = *(float*)val;
	return new Number(-aVal);
}

Object* Number::add(Object* b) {
	float aVal = *(float*)val;
	float bVal = *(float*)b->getVal();
	return new Number(aVal + bVal);
}

Object* Number::sub(Object* b) {
	float aVal = *(float*)val;
	float bVal = *(float*)b->getVal();
	return new Number(aVal - bVal);
}

Object* Number::mult(Object* b) {
	float aVal = *(float*)val;
	float bVal = *(float*)b->getVal();
	return new Number(aVal * bVal);
}

Object* Number::div(Object* b) {
	float aVal = *(float*)val;
	float bVal = *(float*)b->getVal();
	return new Number(aVal / bVal);
}