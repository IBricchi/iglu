#include "LinkedUnoFn.h"

#include "../Chunk.h"

LinkedUnoFn::LinkedUnoFn(Object::unoFn fn) : Function{true} {
	type = "Linked uno function";
	this->airity = 1;
	this->fn = fn;

	chunk->writeOp(0, OpCode::UNARY_FUNC_CALL);
	chunk->writeOp(0, OpCode::RETURN);
}

void LinkedUnoFn::setupLinks() {
	addProperty("__to_string__", &toString);
}

Object* LinkedUnoFn::callFn(Object* a) {
	return (a->*fn)();
}

Object* LinkedUnoFn::toStringFn() {
	return new Str("<Core iglu function>");
}

LinkedUnoFn LinkedUnoFn::toString = LinkedUnoFn((Object::unoFn) & LinkedUnoFn::toStringFn);