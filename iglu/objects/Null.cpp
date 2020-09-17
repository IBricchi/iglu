#include "Null.h"

#include "Str.h"
#include "Function.h"

using namespace std;

Null::Null(): Object{} {
	type = "null";

	addProperty("__to_string__", &toString);
}

Object* Null::toStringFn() {
	return new Str("null");
}
LinkedUnoFn Null::toString = LinkedUnoFn((Object::unoFn) &Null::toStringFn);