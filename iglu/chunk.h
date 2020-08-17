#pragma once

#include <vector>

enum class OpCode {
	OP_RETURN,

};

struct Chunk
{
private:
	std::vector<uint8_t> code;
	std::vector<int> lines;
public:
	Chunk();
	void writeChunk(uint8_t, int);
};

