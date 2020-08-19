#pragma once

#include <vector>

#include "common.h"

#include "Object.h"

enum struct OpCode {
	// objects
	OBJECT,
	
	// arithmetic
	NEGATE,
	PLUS,
	MINUS,
	STAR,
	SLASH,

	// call cpp function
	UNARY_FUNC_CALL,
	BINARY_FUNC_CALL,

	// return
	RETURN
};

struct Chunk
{
	std::vector<uint8_t> code;
	std::vector<int> lines;
	std::vector<Object*> objects;
	Chunk();
	void writeByte(int, uint8_t);
	void writeOp(int, OpCode);
	void writeOpByte(int, OpCode, uint8_t);
};

