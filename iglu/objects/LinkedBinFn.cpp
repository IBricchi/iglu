#include "LinkedBinFn.h"

LinkedBinFn::LinkedBinFn(Object::binFn fn) : Function{true} {
	type = "Linked bin function";
	this->airity = 2;
	this->fn = fn;
}

void LinkedBinFn::setupLinks() {
	addProperty("__to_string__", &toString);
}

Object* LinkedBinFn::toStringFn() {
	return new Str("<Core iglu function>");
}

LinkedUnoFn LinkedBinFn::toString = LinkedUnoFn((Object::unoFn) & LinkedBinFn::toStringFn);
