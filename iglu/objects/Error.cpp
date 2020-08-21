#include "Error.h"

using namespace std;

Error::Error(string message) {
	type = "Error";
	this->message = message;
}

string Error::getMessage() {
	return message;
}