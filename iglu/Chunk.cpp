#include "Chunk.h"

using namespace std;

Chunk::Chunk() {
	code = vector<uint8_t>();
	lines = vector<int>();
}

void Chunk::writeChunk(uint8_t op, int line) {
	code.push_back(op);
	lines.push_back(line);
}