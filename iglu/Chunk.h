#pragma once

#include <vector>
#include <unordered_map>

#include "common.h"

#include "Object.h"
#include "Constant.h"

enum struct OpCode {
	// objects
	CONSTANT,
	POP_STACK,
	
	// arithmetic
	NEGATE,
	PLUS,
	MINUS,
	STAR,
	SLASH,

	// boolean
	EQUAL_EQUAL,
	BANG_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESS,
	LESS_EQUAL,
	BANG,

	// call cpp function
	UNARY_FUNC_CALL,
	BINARY_FUNC_CALL,

	// variables
	DECLARE_VAR,
	DEFINE_VAR,
	GET_VAR,
	GET_MEMBER,

	// return
	RETURN
};

struct Chunk
{
	std::vector<uint8_t> code;
	std::vector<int> lines;
	std::vector<Constant> constants;
	std::vector<Object*> objects;
	std::unordered_map<std::string, uint8_t> variable;
	Chunk();
	void writeByte(int, uint8_t);
	void writeOp(int, OpCode);
	void writeOpByte(int, OpCode, uint8_t);
};

