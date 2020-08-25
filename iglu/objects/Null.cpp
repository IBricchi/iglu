#include "Null.h"

using namespace std;

Null::Null(): Object{} {
	type = "null";
}

string Null::toString() {
	return "null";
}