#include "VM.h"

#include <iostream>

#include "objects/Number.h"
#include "objects/Error.h"

using namespace std;

// API

VM::VM() {
	chunks = stack<Chunk*>();
	pc = stack<uint8_t*>();
	objStack = stack<Object*>();
	hadError = false;
}

void VM::interpret(Chunk* chunk) {
	chunks.push(chunk);
	pc.push(&chunk->code[0]);
	return run();
}


// control

void VM::run() {
	hadError = false;
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
		case OpCode::NEGATE:
		{
			Object* a = topStack();
			if (!callFunction(a, "__negate__")) runtimeError("No overload for the '(-)' operator exsits for type: " + a->getType() + ".");
			break;
		}
		case OpCode::PLUS:
		{
			Object* a = topStack();
			if (!callFunction(a, "__plus__")) runtimeError("No overload for the '+' operator exsits for type: " + a->getType() + ".");
			break;
		}
		case OpCode::MINUS:
		{
			Object* a = topStack();
			if (!callFunction(a, "__minus__")) runtimeError("No overload for the '-' operator exsits for type: " + a->getType() + ".");
			break;
		}
		case OpCode::STAR:
		{
			Object* a = topStack();
			if (!callFunction(a, "__star__")) runtimeError("No overload for the '*' operator exsits for type: " + a->getType() + ".");
			break;
		}
		case OpCode::SLASH:
		{
			Object* a = topStack();
			if (!callFunction(a, "__slash__")) runtimeError("No overload for the '/' operator exsits for type: " + a->getType() + ".");
			break;
		}
		case OpCode::UNARY_FUNC_CALL:
		{
			Object* a = popStack();
			uint8_t fi = readByte();
			Object* b = (a->*a->unoFns[fi])();

			// check if error object is returned
			if (b->checkType("Error") >= 0) runtimeErrorObject(b);

			pushStack(b);
			break;
		}
		case OpCode::BINARY_FUNC_CALL:
		{
			Object* b = popStack();
			Object* a = popStack();
			uint8_t fi = readByte();
			Object* c = (a->*(a->binFns[fi]))(b);

			// check if error object is returned
			if (c->checkType("Error") >= 0) runtimeErrorObject(c);
			
			pushStack(c);
			break;
		}
		case OpCode::RETURN:
			leaveChunk();
			break;
		}
	}
	cout << ((Number*)objStack.top())->getVal() << endl;
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

inline Object* VM::topStack() {
	return objStack.top();
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

bool VM::callFunction(Object* obj, string name) {
	auto it = obj->properties.find(name);
	if (it != obj->properties.end()) {
		intoChunk(it->second);
		return true;
	}
	return false;
}

void VM::runtimeError(string message) {
	hadError = true;

	// print error message
	int line = chunks.top()->lines[pc.top() - &chunks.top()->code[0] - 1];
	cerr << "[Line: " << line << "] " << message << endl;
	
	// clear stacks
	while (!chunks.empty()) chunks.pop();
	while (!pc.empty()) pc.pop();
}

inline void VM::runtimeErrorObject(Object* errorObj) {
	runtimeError(((Error*)errorObj)->getMessage());
}
