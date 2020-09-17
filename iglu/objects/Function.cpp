#include "Function.h"

#include "Str.h"
#include "Null.h"

// TODO remove this and fix call function for custom functions written in Iglu
#include <iostream>

using namespace std;

Function::Function() : Object{} {
	type = "Function";
	airity = 0;
	this->chunk = new Chunk();

	addProperty("__to_string__", &toString);
	
	addProperty("__call__", &call);
}

Function::Function(bool _) : Object{ _ } {
	type = "Function";
	airity = 0;
	this->chunk = new Chunk();

	linkedFns.push_back(this);
}

Function::Function(Chunk* chunk) : Object{} {
	airity = 0;
	this->chunk = chunk;

	addProperty("__to_string__", &toString);

	addProperty("__call__", &call);
}

Function::~Function() {
	delete chunk;
}

Object* Function::toStringFn() {
	return new Str("Function");
}
LinkedUnoFn Function::toString = LinkedUnoFn((Object::unoFn) &Function::toStringFn);

Object* Function::callFn() {
	cout << "Call used on " << this->type << " by variable " << currentReference.front();
	return new Null();
}
LinkedUnoFn Function::call = LinkedUnoFn((Object::unoFn) &Function::callFn);

Chunk* Function::getChunk() {
	return chunk;
}

vector<Function*> Function::linkedFns = vector<Function*>();
void Function::linkFns() {
	for (int i = 0; i < linkedFns.size(); i++) {
		linkedFns[i]->setupLinks();
	}
}
void Function::setupLinks() {
	throw exception("Cannot call setupLinks on a non linked function");
}