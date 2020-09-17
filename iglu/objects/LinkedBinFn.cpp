#include "LinkedBinFn.h"

LinkedBinFn::LinkedBinFn(Object::binFn fn) : Function{true} {
	type = "Linked bin function";
	this->airity = 2;
	this->fn = fn;

	chunk->writeOp(0, OpCode::BINARY_FUNC_CALL);
	chunk->writeOp(0, OpCode::RETURN);
}

void LinkedBinFn::setupLinks() {
	addProperty("__to_string__", &toString);
}

Object* LinkedBinFn::callFn(Object* a, Object* b) {
	return (a->*fn)(b);
}

Object* LinkedBinFn::toStringFn() {
	return new Str("<Core iglu function>");
}

LinkedUnoFn LinkedBinFn::toString = LinkedUnoFn((Object::unoFn) & LinkedBinFn::toStringFn);
