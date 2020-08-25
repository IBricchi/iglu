#include "Object.h"

using namespace std;

Object::Object() {
	type = "Object";
	properties = unordered_map<string, Chunk*>();
	binFns = vector<Object::binFn>();

	references = unordered_multiset<string>();
	currentReference = queue<string>();
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

string Object::toString() {
	return "Object";
}

void Object::reference(string var) {
	currentReference.push(var);
}

string Object::dereference() {
	if (currentReference.empty()) return "";
	string front = currentReference.front();
	currentReference.pop();
	return front;
}

void Object::addReference(string var) {
	references.emplace(var);
}

void Object::removeReference(string var) {
	references.erase(references.find(var));
	if (references.empty()) {
		delete this;
	}
}

void Object::giveImortality() {
	references.emplace("#");
}

void Object::removeImortality() {
	references.erase(references.find("#"));
	if (references.empty()) {
		delete this;
	}
}
