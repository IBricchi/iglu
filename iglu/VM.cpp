#include <iostream>

#include "VM.h"

using namespace std;

// API

VM::VM() {
	chunks = stack<Chunk*>();
	pc = stack<uint8_t*>();
	objStack = stack<Object*>();
}

InterpreterResults VM::interpret(Chunk* chunk) {
	chunks.push(chunk);
	pc.push(&chunk->code[0]);
	return run();
}


// control

InterpreterResults VM::run() {
	InterpreterResults out = InterpreterResults::OK;
	while (!chunks.empty()) {
		OpCode instruction = (OpCode)readByte();
		switch (instruction)
		{
		case OpCode::OBJECT:
		{
			Object* obj = readObject();
			objStack.push(obj);
			break;
		}
		case OpCode::ADD:
		{
			Object* a = popStack();
			Object* b = popStack();
			intoChunk(a->plus(this, b));
			break;
		}
		case OpCode::RETURN:
			cout << *((int*)objStack.top()->val) << endl;
			leaveChunk();
			break;
		}
	}
	return out;
}

// helpers

inline uint8_t VM::readByte() {
	return *(pc.top()++);
}

inline Object* VM::readObject() {
	return chunks.top()->objects[*(pc.top()++)];
}

inline Object* VM::popStack() {
	Object* poped = objStack.top();
	objStack.pop();
	return poped;
}

inline void VM::pushStack(Object* obj) {
	objStack.push(obj);
}

void VM::intoChunk(Chunk* chunk) {
	chunks.push(chunk);
	pc.push(&chunk->code[0]);
}

void VM::leaveChunk() {
	pc.pop();
	chunks.pop();
}