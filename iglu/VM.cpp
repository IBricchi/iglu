#include "VM.h"

#include <iostream>

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
		case OpCode::NEGATE:
		{
			Object* a = topStack();
			if (!callFunction(a, "__negate__")) {
				out = InterpreterResults::RUN_ERR;
				runtimeError("No overload for the '(-)' operator exsits for type: " + a->type + ".");
			}
			break;
		}
		case OpCode::PLUS:
		{
			Object* a = topStack();
			if (!callFunction(a, "__plus__")) {
				out = InterpreterResults::RUN_ERR;
				runtimeError("No overload for the '+' operator exsits for type: " + a->type + ".");
			}
			break;
		}
		case OpCode::MINUS:
		{
			Object* a = topStack();
			if (!callFunction(a, "__minus__")) {
				out = InterpreterResults::RUN_ERR;
				runtimeError("No overload for the '-' operator exsits for type: " + a->type + ".");
			}
			break;
		}
		case OpCode::STAR:
		{
			Object* a = topStack();
			if (!callFunction(a, "__star__")) {
				out = InterpreterResults::RUN_ERR;
				runtimeError("No overload for the '*' operator exsits for type: " + a->type + ".");
			}
			break;
		}
		case OpCode::SLASH:
		{
			Object* a = topStack();
			if (!callFunction(a, "__slash__")) {
				out = InterpreterResults::RUN_ERR;
				runtimeError("No overload for the '/' operator exsits for type: " + a->type + ".");
			}
			break;
		}
		case OpCode::UNARY_FUNC_CALL:
		{
			Object* a = popStack();
			uint8_t fi = readByte();
			Object* b = (a->*a->unoFns[fi])();
			pushStack(b);
			break;
		}
		case OpCode::BINARY_FUNC_CALL:
		{
			Object* b = popStack();
			Object* a = popStack();
			uint8_t fi = readByte();
			Object* c = (a->*(a->binFns[fi]))(b);
			pushStack(c);
			break;
		}
		case OpCode::RETURN:
			leaveChunk();
			break;
		}
	}
	cout << *(float*)objStack.top()->getVal() << endl;
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
	int line = chunks.top()->lines[pc.top() - &chunks.top()->code[0] - 1];
	cerr << "[Line: " << line << "] " << message << endl;
	while (!chunks.empty()) chunks.pop();
	while (!pc.empty()) pc.pop();
}