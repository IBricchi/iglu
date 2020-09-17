#include "Str.h"

#include "Bool.h"
#include "Error.h"
#include "Function.h"

using namespace std;

Str::Str(string val) : Object{} {
	this->val = val;
	type = "String";

	addProperty("__to_string__", &toString);

	addProperty("__plus__", &concat);
	addProperty("__equal_equal__", &compare);
	addProperty("__not_equal__", &notCompare);
}

Str::Str(string* val) : Object{} {
	this->val = *val;
	type = "String";

	addProperty("__to_string__", &toString);

	addProperty("__plus__", &concat);
	addProperty("__equal_equal__", &compare);
	addProperty("__not_equal__", &notCompare);
}

Object* Str::toStringFn(){
	return new Str(val);
}
LinkedUnoFn Str::toString = LinkedUnoFn((Object::unoFn) &Str::toStringFn);

Object* Str::concatFn(Object* b) {
	if (b->checkType("String") != 0) return new Error("String does not support concatenation wtih " + b->getType() + ".");
	string bVal = ((Str*)b)->getVal();
	return new Str(val + bVal);
}
LinkedBinFn Str::concat = LinkedBinFn((Object::binFn) &Str::concatFn);

Object* Str::compareFn(Object* b) {
	if (b->checkType("String") != 0) return new Error("String does not support comparing wtih " + b->getType() + ".");
	string bVal = ((Str*)b)->getVal();
	return new Bool(val == bVal);
}
LinkedBinFn Str::compare = LinkedBinFn((Object::binFn) &Str::compareFn);

Object* Str::notCompareFn(Object* b) {
	if (b->checkType("String") != 0) return new Error("String does not support comparing wtih " + b->getType() + ".");
	string bVal = ((Str*)b)->getVal();
	return new Bool(val != bVal);
}
LinkedBinFn Str::notCompare = LinkedBinFn((Object::binFn) &Str::notCompareFn);

string Str::getVal() {
	return val;
}