#include "Compiler.h"

#include "objects/Number.h"
#include "objects/Null.h"

using namespace std;

// API

Compiler::Compiler() {
	chunk = Chunk();
	rpn = nullptr;
}

Chunk* Compiler::getChunk() {
	return &chunk;
}

void Compiler::compile(queue<Token>* rpn) {
	this->rpn = rpn;
	Token rt{};
	rt.line = 0;
	rt.type = TokenType::RETURN;
	this->rpn->push(rt);
	chunk = Chunk();
	statement();
}

// helpers

Token Compiler::popRpn() {
	Token top = rpn->front();
	rpn->pop();
	return top;
}

inline Token Compiler::frontRpn() {
	return rpn->front();
}

void Compiler::tokenToChunk(Token token) {
	switch (token.type)
	{
	case TokenType::PLUS:
		chunk.writeOp(token.line, OpCode::PLUS);
		break;
	case TokenType::MINUS:
		chunk.writeOp(token.line, OpCode::MINUS);
		break;
	case TokenType::STAR:
		chunk.writeOp(token.line, OpCode::STAR);
		break;
	case TokenType::SLASH:
		chunk.writeOp(token.line, OpCode::SLASH);
		break;
	case TokenType::NEGATE:
		chunk.writeOp(token.line, OpCode::NEGATE);
		break;
	case TokenType::NUMBER: {
		string strVal = string(token.start, token.start + token.length);
		float val = stof(strVal);
		Object* num = new Number(val);
		num->giveImortality();
		chunk.objects.push_back(num);
		chunk.writeOpByte(token.line, OpCode::OBJECT, chunk.objects.size() - 1);
		break;
	}
	case TokenType::LET: {
		Token var = frontRpn();
		string varName = string(var.start, var.start + var.length);
		chunk.objects.push_back(new Null());
		chunk.objects.back()->giveImortality();
		chunk.writeOpByte(token.line, OpCode::DECLARE_VAR, chunk.variable.size() - 1);
		break;
	}
	//case TokenType::IDENTIFIER: {
	//	chunk.writeOpByte(token.line, OpCode::GET_VAR, )
	//}
	case TokenType::RETURN:
		chunk.writeOp(token.line, OpCode::RETURN);
		break;
	}
}

// token specifics

void Compiler::statement() {
	expression();
}

void Compiler::expression() {
	Token token = popRpn();
	while (!rpn->empty()) {
		tokenToChunk(token);
		token = popRpn();
	}
	tokenToChunk(token);
}