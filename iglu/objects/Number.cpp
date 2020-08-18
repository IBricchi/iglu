#include "Number.h"

Number::Number(float val) {
	this->val = new float(val);

	Chunk* plus = new Chunk();
	plus->code.push_back((uint8_t) OpCode::BINARY_FUNC_CALL);
	plus->code.push_back(0);
	plus->code.push_back((uint8_t) OpCode::RETURN);

	this->binaryFuncs.push_back(add);

	this->properties.insert({"__plus__", plus});
}

Object* Number::add(Object* a, Object* b) {
	float aVal = *(float*)a->getVal();
	float bVal = *(float*)b->getVal();

	return new Number(aVal + bVal);
}