#include "Object.h"

using namespace std;

Object::Object() {
	type = "Object";
	properties = map<string, Chunk*>();
	binFns = vector<Object::binFn>();
}

Object::~Object() {
	for (auto i = properties.begin(); i != properties.end(); i++) {
		delete i->second;
	}
}

Chunk* Object::generateFnChunk(unoFn fn) {
	Chunk* chunk = new Chunk();
	chunk->writeOpByte(0, OpCode::UNARY_FUNC_CALL, unoFns.size());
	chunk->writeOp(0, OpCode::RETURN);
	unoFns.push_back(fn);
	return chunk;
}

Chunk* Object::generateFnChunk(binFn fn) {
	Chunk* chunk = new Chunk();
	chunk->writeOpByte(0, OpCode::BINARY_FUNC_CALL, binFns.size());
	chunk->writeOp(0, OpCode::RETURN);
	binFns.push_back(fn);
	return chunk;
}

void Object::addFnProperty(string prop, Object::unoFn fn) {
	properties[prop] = generateFnChunk(fn);
}

void Object::addFnProperty(string prop, Object::binFn fn) {
	properties[prop] = generateFnChunk(fn);
}

string Object::getType() {
	return type;
}

int Object::checkType(string type) {
	if(this->type == type) return 0;
	return -1;
}