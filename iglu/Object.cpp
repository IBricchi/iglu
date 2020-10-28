#include "Object.h"
#include "objects/Str.h"
#include "objects/Function.h"

using namespace std;

Object::Object() {
	type = "Object";
	currentReference = queue<string>();
	marked = false;
	properties = unordered_map<string, Object*>();

	addProperty("__to_string__", &toString);
}

Object::Object(bool) {
	type = "Object";
	currentReference = queue<string>();
	marked = false;
	properties = unordered_map<string, Object*>();
}

Chunk* Object::generateUnoFnChunk(int index) {
	Chunk* chunk = new Chunk();
	chunk->writeOpByte(0, OpCode::UNARY_FUNC_CALL, index);
	chunk->writeOp(0, OpCode::RETURN);
	
	return chunk;
}

Chunk* Object::generateBinFnChunk(int index) {
	Chunk* chunk = new Chunk();
	chunk->writeOpByte(0, OpCode::BINARY_FUNC_CALL, index);
	chunk->writeOp(0, OpCode::RETURN);

	return chunk;
}

void Object::addFnProperty(string prop, Object::unoFn fn) {
	Object* lf = new LinkedUnoFn(fn);
	addProperty(prop, lf);
}

void Object::addFnProperty(string prop, Object::binFn fn) {
	Object* lf = new LinkedBinFn(fn);
	addProperty(prop, lf);
}

void Object::addProperty(string prop, Object* obj) {
	auto it = properties.find(prop);
	if (it != properties.end()){
		Object* old = properties[prop];
	}
	properties[prop] = obj;
}

string Object::getType() {
	return type;
}

int Object::checkType(string type) {
	if(this->type == type) return 0;
	return -1;
}

Object* Object::toStringFn() {
	return new Str("Object");
}
LinkedUnoFn Object::toString = LinkedUnoFn((Object::unoFn) &Object::toStringFn);

void Object::reference(string var) {
	currentReference.push(var);
}

string Object::dereference() {
	if (currentReference.empty()) return "";
	string front = currentReference.front();
	currentReference.pop();
	return front;
}

//garbage collection
void Object::mark() {
	if(marked) return;
	marked = true;
	for (auto i : properties) {
		i.second->mark();
	}
}
bool Object::checkMark() {
	return marked;
}
void Object::unmark() {
	marked = false;
}

// debugging shit
string Object::debugToString() {
	return "Object";
}
