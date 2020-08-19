#pragma once

#include <vector>

#include "common.h"

#include "Object.h"

enum struct OpCode {
	OBJECT,
	PLUS,
	BINARY_FUNC_CALL,
	RETURN
};

struct Chunk
{
	std::vector<uint8_t> code;
	std::vector<int> lines;
	std::vector<Object*> objects;
	Chunk();
	void writeChunk(uint8_t, int);
};

