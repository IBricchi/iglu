#include "LinkedUnoFn.h"

LinkedUnoFn::LinkedUnoFn(Object::unoFn fn) : Function{true} {
	type = "Linked uno function";
	this->airity = 1;
	this->fn = fn;
}

void LinkedUnoFn::setupLinks() {
	addProperty("__to_string__", &toString);
}

Object* LinkedUnoFn::toStringFn() {
	return new Str("<Core iglu function>");
}

LinkedUnoFn LinkedUnoFn::toString = LinkedUnoFn((Object::unoFn) & LinkedUnoFn::toStringFn);