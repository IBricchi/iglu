#include "Null.h"

#include "Str.h"

using namespace std;

Null::Null(): Object{} {
	type = "null";

	addFnProperty("__to_string__", (Object::unoFn) &Null::toString);
}

Object* Null::toString() {
	return new Str("null");
}