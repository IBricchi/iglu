#include "Error.h"

using namespace std;

Error::Error(string message) : Object{} {
	type = "Error";
	this->message = message;
}

string Error::getMessage() {
	return message;
}