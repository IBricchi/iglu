#include "Object.h"
#include "objects/Str.h"

using namespace std;

Object::Object() {
	type = "Object";
	properties = unordered_map<string, pair<int, Chunk*>>();
	unoFns = vector<Object::unoFn>();
	binFns = vector<Object::binFn>();

	references = unordered_multiset<string>();
	currentReference = queue<string>();

	addFnProperty("__to_string__", (Object::unoFn) &Object::toString);
}

Object::~Object() {
	for (auto i = properties.begin(); i != properties.end(); i++) {
		delete i->second.second;
	}
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
	int index;
	if (properties.find(prop) == properties.end()) {
		index = unoFns.size();
		unoFns.push_back(fn);
	}
	else {
		index = properties[prop].first;
		unoFns[index] = fn;

		delete properties[prop].second;
	}
	properties[prop] = pair<int, Chunk*>{index, generateUnoFnChunk(index)};
}

void Object::addFnProperty(string prop, Object::binFn fn) {
	int index;
	if (properties.find(prop) == properties.end()) {
		index = binFns.size();
		binFns.push_back(fn);
	}
	else {
		index = properties[prop].first;
		binFns[index] = fn;

		delete properties[prop].second;
	}

	properties[prop] = pair<int, Chunk*>{index, generateBinFnChunk(index)};
}

string Object::getType() {
	return type;
}

int Object::checkType(string type) {
	if(this->type == type) return 0;
	return -1;
}

Object* Object::toString() {
	return new Str("Object");
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
