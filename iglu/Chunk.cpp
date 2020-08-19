#include "Chunk.h"

#include <cstdarg>

using namespace std;

Chunk::Chunk() {
	code = vector<uint8_t>();
	lines = vector<int>();
}

void Chunk::writeByte(int line, uint8_t byte) {
	code.push_back(byte);
	lines.push_back(line);
}

void Chunk::writeOp(int line, OpCode op) {
	writeByte(line, (uint8_t) op);
}

void Chunk::writeOpByte(int line, OpCode op, uint8_t byte) {
	writeOp(line, op);
	writeByte(line, byte);
}