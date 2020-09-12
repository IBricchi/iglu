#include "Error.h"

#include "Str.h"

using namespace std;

Error::Error(string message) : Object{} {
	type = "Error";
	this->message = message;

	addFnProperty("__to_string__", (Object::unoFn) &Error::toString);
}

string Error::getMessage() {
	return message;
}

Object* Error::toString() {
	return new Str("Error ("+message+")");
}