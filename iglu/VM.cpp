#include "VM.h"

#include <iostream>

#include "objects/Number.h"
#include "objects/Null.h"
#include "objects/Error.h"

using namespace std;

// API

VM::VM() {
	chunks = vector<Chunk*>();
	pc = vector<uint8_t*>();
	stack = vector<Object*>();
	hadError = false;
}

void VM::interpret(Chunk* chunk) {
	chunks.push_back(chunk);
	pc.push_back(&chunk->code[0]);
	return run();
}


// control

void VM::run() {
	hadError = false;
	while (!chunks.empty()) {
		OpCode instruction = (OpCode)readByte();
		switch (instruction)
		{
		case OpCode::CONSTANT:{
			Constant constant = readConstant();
			switch (constant.type)
			{
			case ConstType::NILL: {
				stack.push_back(new Null());
				stack.back()->giveImortality();
				break;
			}
			case ConstType::BOOL: {
				// TODO! add bools
				break;
			}
			case ConstType::NUMBER: {
				stack.push_back(new Number(constant.val.Number));
				stack.back()->giveImortality();
				break;
			}
			case ConstType::STRING: {
				// TODO! add strings
				break;
			}
			}
			break;
		}
		case OpCode::OBJECT:
		{
			Object* obj = readObject();
			stack.push_back(obj);
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
	cout << ((Number*)stack.back())->getVal() << endl;
}

// helpers

inline uint8_t VM::readByte() {
	return *(pc.back()++);
}

inline Constant VM::readConstant() {
	return chunks.back()->constants[*(pc.back()++)];
}

inline Object* VM::readObject() {
	return chunks.back()->objects[*(pc.back()++)];
}

inline Object* VM::popStack() {
	Object* poped = stack.back();
	stack.pop_back();
	return poped;
}

inline Object* VM::topStack() {
	return stack.back();
}

inline void VM::pushStack(Object* obj) {
	stack.push_back(obj);
}

void VM::intoChunk(Chunk* chunk) {
	chunks.push_back(chunk);
	pc.push_back(&chunk->code[0]);
}

void VM::leaveChunk() {
	pc.pop_back();
	chunks.pop_back();
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
	int line = chunks.back()->lines[pc.back() - &chunks.back()->code[0] - 1];
	cerr << "[Line: " << line << "] " << message << endl;
	
	// clear stacks
	while (!chunks.empty()) chunks.pop_back();
	while (!pc.empty()) pc.pop_back();
}

inline void VM::runtimeErrorObject(Object* errorObj) {
	runtimeError(((Error*)errorObj)->getMessage());
}
