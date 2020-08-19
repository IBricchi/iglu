#include "Object.h"

using namespace std;

Object::Object() {
	properties = map<string, Chunk*>();
	binFns = vector<Object::binFn>();
}

Object::~Object() {
	delete val;
	for (auto i = properties.begin(); i != properties.end(); i++) {
		delete i->second;
	}
}

void* Object::getVal() {
	return val;
}