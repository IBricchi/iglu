#include "Str.h"

#include "Number.h"
#include "Error.h"

using namespace std;

Str::Str(string val) : Object{} {
	this->val = val;
	type = "String";

	addFnProperty("__plus__", (Object::binFn) & Str::concat);
}

Str::Str(string* val) : Object{} {
	this->val = *val;
	type = "String";

	addFnProperty("__plus__", (Object::binFn) &Str::concat);
}

Object* Str::concat(Object* b) {
	string bVal = b->toString();
	return new Str(val + bVal);
}

string Str::toString(){
	return val;
}

string Str::getVal() {
	return val;
}