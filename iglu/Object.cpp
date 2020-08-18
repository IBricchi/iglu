#include "Object.h"

Object::~Object() {
	delete val;
}

Chunk* Object::plus(VM* vm, Object* obj) {
	Chunk* chunk = new Chunk();

	Object* c = new Object();
	c->val = new int( *((int*)this->val) + *((int*)obj->val) );
	chunk->objects.push_back(c);

	chunk->code.push_back((uint8_t)OpCode::OBJECT);
	chunk->code.push_back(0);

	chunk->code.push_back((uint8_t)OpCode::RETURN);

	return chunk;
}