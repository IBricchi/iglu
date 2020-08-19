#include "Number.h"

Number::Number(float val) {
	this->val = new float(val);

	Chunk* plus = new Chunk();
	plus->code.push_back((uint8_t) OpCode::BINARY_FUNC_CALL);
	plus->code.push_back(0);
	plus->code.push_back((uint8_t) OpCode::RETURN);

	this->binFns.push_back((Object::binFn) &Number::add);

	this->properties.insert({"__plus__", plus});
}

Object* Number::add(Object* b) {
	float aVal = *(float*)val;
	float bVal = *(float*)b->getVal();

	return new Number(aVal + bVal);
}