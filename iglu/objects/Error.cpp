#include "Error.h"

#include "Str.h"
#include "Function.h"

using namespace std;

Error::Error(string message) : Object{} {
	type = "Error";
	this->message = message;

	addProperty("__to_string__", &toString);
}

string Error::getMessage() {
	return message;
}

Object* Error::toStringFn() {
	return new Str("Error ("+message+")");
}
LinkedUnoFn Error::toString = LinkedUnoFn((Object::unoFn) &Error::toStringFn);

// debugging shit
string Error::debugToString() {
	return "Error (" + message + ")";
}