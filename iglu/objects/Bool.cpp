#include "Bool.h"

#include "Number.h"
#include "Error.h"

using namespace std;

Bool::Bool(bool val) : Object{} {
	this->val = val;
	type = "Bool";
}

string Bool::toString() {
	return val?"true":"false";
}

bool Bool::getVal() {
	return val;
}