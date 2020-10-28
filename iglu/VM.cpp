#include "VM.h"

#include <iostream>

#include "objects/Bool.h"
#include "objects/Error.h"
#include "objects/Function.h"
#include "objects/Number.h"
#include "objects/Null.h"
#include "objects/Str.h"

using namespace std;


// API
VM::VM() {
	chunks = vector<Chunk*>();
	pc = vector<uint8_t*>();
	stack = vector<RefObj>();
	objs = unordered_set<Object*>();
	hadError = false;

	Function::linkFns();
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
		// Constants
		case OpCode::CONSTANT:{
			Constant constant = readConstant();
			switch (constant.type)
			{
			case ConstType::NILL: {
				pushStack(new Null());
				break;
			}
			case ConstType::BOOL: {
				pushStack(new Bool(constant.as.Bool));
				break;
			}
			case ConstType::NUMBER: {
				pushStack(new Number(constant.as.Number));
				break;
			}
			case ConstType::STRING: {
				pushStack(new Str(constant.as.String));
				break;
			}
			}
			objs.insert(topStackObj());
			break;
		}
		// arithmetic
		case OpCode::NEGATE:{
			Object* a = topStackObj();
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__negate__", "(-)");
			break;
		}
		case OpCode::PLUS:{
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__plus__", "+");
			break;
		}
		case OpCode::MINUS:{
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__minus__", "-");
			break;
		}
		case OpCode::STAR:{
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__star__", "*");
			break;
		}
		case OpCode::SLASH:{
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__slash__", "/");
			break;
		}
		// boolean
		case OpCode::BANG: {
			Object* a = topStackObj();
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__not__", "!");
			break;
		}
		case OpCode::EQUAL_EQUAL: {
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__equal_equal__", "==");
			break;
		}
		case OpCode::BANG_EQUAL: {
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__not_equal__", "!=");
			break;
		}
		case OpCode::GREATER: {
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__greater__", ">");
			break;
		}
		case OpCode::GREATER_EQUAL: {
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__greater_equal__", ">=");
			break;
		}
		case OpCode::LESS: {
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__less__", "<");
			break;
		}
		case OpCode::LESS_EQUAL: {
			Object* a = stackAtObj(1);
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			tryCallFunction(a, "__less_equal__", "<=");
			break;
		}
		// cpp functions
		case OpCode::UNARY_FUNC_CALL:{
			LinkedUnoFn* fn = (LinkedUnoFn*) popStackObj();
			if (fn->checkType("Error") >= 0) { runtimeErrorObject(fn); break; }

			Object* a = popStackObj();
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }

			Object* b = fn->callFn(a);

			// check if error object is returned
			if (b->checkType("Error") >= 0) runtimeErrorObject(b);
			else {
				pushStack(b);
				objs.insert(b);
			}
			
			break;
		}
		case OpCode::BINARY_FUNC_CALL:{
			LinkedBinFn* fn = (LinkedBinFn*) popStackObj();
			if (fn->checkType("Error") >= 0) { runtimeErrorObject(fn); break; }

			Object* b = popStackObj();
			if (b->checkType("Error") >= 0) { runtimeErrorObject(b); break; }

			Object* a = popStackObj();
			if (a->checkType("Error") >= 0) { runtimeErrorObject(a); break; }


			Object* c = fn->callFn(a, b);

			// check if error object is returned
			if (c->checkType("Error") >= 0) runtimeErrorObject(c);
			else {
				pushStack(c);
				objs.insert(c);
			}

			break;
		}
		// variables
		case OpCode::DECLARE_VAR:{
			string* name = readConstant().as.String;
			if (variables.find(*name) != variables.end()) { // TODO! come back when different scopes exists
				runtimeError("Variable '" + *name + "' has already been declared.");
				break;
			}
			Object* nullObj = new Null();
			objs.insert(nullObj);
			if(variables.find(*name) == variables.end()) variables.emplace(*name, vector<Object*>{nullObj}); // this will be necessary later
			else variables[*name].push_back(nullObj);
			break;
		}
		case OpCode::DEFINE_VAR: {
			Object* b = popStackObj();
			if (b->checkType("Error") >= 0) { runtimeErrorObject(b); break; }

			string name = popStackRef();
			
			if(variables.find(name) == variables.end()){
				runtimeError("Cannot assign to undeclared variable '" + name + "'.");
				break;
			}

			vector<Object*>* varVec = &variables[name];
			(*varVec)[varVec->size() - 1] = b;

			Object* nullObj = new Null();

			pushStack(nullObj);
			
			break;
		}
		case OpCode::GET_VAR: { // TODO! change from get_var to identifier
			string* name = readConstant().as.String;
			pushStack(name);
			break;
		}
		case OpCode::GET_MEMBER: {
			string member = popStackRef();
			Object* a = topStackObj();
			if(a->checkType("Error") >= 0) {runtimeErrorObject(a); break;}

			string name = popStackRef();
			
			if (a->properties.find(member) == a->properties.end()) {
				runtimeError("Variable '" + name + "' has no propery '" + member + "'.");
				break;
			}
			pushStack(a->properties[member]);
			break;
		}
		// return
		case OpCode::RETURN:
			leaveChunk();
			break;
		case OpCode::POP_STACK: {
			Object* out = popStackObj();
			if(out->checkType("Error") >= 0) {runtimeErrorObject(out); break; }
			
			// debuging
			cout << out->getType() << ": " << out->debugToString() << endl;
			
			break;
		}
		}
	}
}

// helpers

inline uint8_t VM::readByte() {
	return *(pc.back()++);
}

inline Constant VM::readConstant() {
	return chunks.back()->constants[*(pc.back()++)];
}

inline string VM::popStackRef() {
	RefObj popped = stack.back();
	stack.pop_back();
	return *popped.ref;
}
Object* VM::popStackObj() {
	RefObj ro = stack.back();
	stack.pop_back();

	if (ro.obj == nullptr) {
		string name = *ro.ref;
		auto it = variables.find(name);
		if (it == variables.end()) {
			Object* error = new Error("Variable '" + name + "' has not been declared.");
			pushStack(error);
			return error;
		}
		ro.obj = it->second.back();
	}

	return ro.obj;
}

inline string VM::topStackRef() {
	return *stack.back().ref;
}
Object* VM::topStackObj() {
	RefObj ro = stack.back();

	if (ro.obj == nullptr) {
		string name = *ro.ref;
		auto it = variables.find(name);
		if (it == variables.end()) {
			Object* error = new Error("Variable '" + name + "' has not been declared.");
			pushStack(error);
			return error;
		}
		ro.obj = it->second.back();
	}

	return ro.obj;
}

inline string VM::stackAtRef(int n) {
	return *(stack.end() - 1 - n)->ref;
}
Object* VM::stackAtObj(int n) {
	RefObj ro = *(stack.end() - 1 - n);

	if (ro.obj == nullptr) {
		string name = *ro.ref;
		auto it = variables.find(name);
		if (it == variables.end()) {
			Object* error = new Error("Variable '" + name + "' has not been declared.");
			pushStack(error);
			return error;
		}
		ro.obj = it->second.back();
	}

	return ro.obj;
}

inline void VM::pushStack(string* ref) {
	stack.push_back(RefObj{ref,nullptr});
}
inline void VM::pushStack(Object* obj) {
	stack.push_back(RefObj{nullptr,obj});
}

void VM::intoChunk(Chunk* chunk) {
	chunks.push_back(new Chunk(*chunk));
	pc.push_back(&chunks.back()->code[0]);
}

void VM::leaveChunk() {
	pc.pop_back();
	chunks.pop_back();
	cleanGarbage(); // TODO! Move to a better location later
}

void VM::cleanGarbage() {
	if(stack.size() > 0 && stack.back().obj != nullptr) topStackObj()->mark();
	for (auto i : variables) {
		for (auto j : i.second) {
			j->mark();
		}
	}
	for (auto i = objs.begin(); i != objs.end();) {
		if (!(*i)->checkMark()) {
			delete *i;
			objs.erase(i++);
		}
		else {
			(*i)->unmark();
			i++;
		}
	}
}

bool VM::callFunction(Object* obj, string name) {
	auto it = obj->properties.find(name);
	if (it != obj->properties.end()) {
		pushStack(it->second);
		intoChunk( ((Function*)it->second)->getChunk() );
		return true;
	}
	return false;
}

void VM::tryCallFunction(Object* obj, string call, string sym) {
	if (!callFunction(obj, call)) runtimeError("No overload for the '" + sym + "' operator exsits for type: " + obj->getType() + ".");
}

void VM::runtimeError(string message) {
	hadError = true;

	// print error message
	int line = chunks.back()->lines[pc.back() - &chunks.back()->code[0] - 1];
	cerr << "[Line: " << line << "] " << message << endl;
	
	// clear stacks
	while (!chunks.empty()) chunks.pop_back();
	while (!pc.empty()) pc.pop_back();
	while (!stack.empty()) stack.pop_back();
}

inline void VM::runtimeErrorObject(Object* errorObj) {
	runtimeError(((Error*)errorObj)->getMessage());
}
