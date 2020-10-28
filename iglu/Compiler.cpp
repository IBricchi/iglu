#include "Compiler.h"

#include "Constant.h"

// debuging
#include "Parser.h"

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
	doCompile();
}

void Compiler::doCompile() {
	Token token = popRpn();
	while (!rpn->empty()) {
		tokenToChunk(token);
		token = popRpn();
	}
	tokenToChunk(token);
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
	// arithmetic
	case TokenType::NEGATE:
		chunk.writeOp(token.line, OpCode::NEGATE);
		break;
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
	//bool
	case TokenType::BANG:
		chunk.writeOp(token.line, OpCode::BANG);
		break;
	case TokenType::EQUAL_EQUAL:
		chunk.writeOp(token.line, OpCode::EQUAL_EQUAL);
		break;
	case TokenType::BANG_EQUAL:
		chunk.writeOp(token.line, OpCode::BANG_EQUAL);
		break;
	case TokenType::GREATER:
		chunk.writeOp(token.line, OpCode::GREATER);
		break;
	case TokenType::GREATER_EQUAL:
		chunk.writeOp(token.line, OpCode::GREATER_EQUAL);
		break;
	case TokenType::LESS:
		chunk.writeOp(token.line, OpCode::LESS);
		break;
	case TokenType::LESS_EQUAL:
		chunk.writeOp(token.line, OpCode::LESS_EQUAL);
		break;
	// constants
	case TokenType::NUMBER: {
		string strVal = string(token.start, token.start + token.length);
		Constant num = stof(strVal);
		chunk.constants.push_back(num);
		chunk.writeOpByte(token.line, OpCode::CONSTANT, chunk.constants.size() - 1);
		break;
	}
	case TokenType::STRING: {
		string strVal = string(token.start + 1, token.start + token.length - 1);
		Constant str = strVal;
		chunk.constants.push_back(str);
		chunk.writeOpByte(token.line, OpCode::CONSTANT, chunk.constants.size() - 1);
		break;
	}
	case TokenType::TRUE: {
		Constant trueConst = Constant(true);
		chunk.constants.push_back(trueConst);
		chunk.writeOpByte(token.line, OpCode::CONSTANT, chunk.constants.size() - 1);
		break;
	}
	case TokenType::FALSE: {
		Constant trueConst = Constant(false);
		chunk.constants.push_back(trueConst);
		chunk.writeOpByte(token.line, OpCode::CONSTANT, chunk.constants.size() - 1);
		break;
	}
	case TokenType::NILL: {
		Constant nullConst = Constant();
		chunk.constants.push_back(nullConst);
		chunk.writeOpByte(token.line, OpCode::CONSTANT, chunk.constants.size() - 1);
		break;
	}
	//variables
	case TokenType::LET: {
		Token var = frontRpn();
		string varName = string(var.start, var.start + var.length);
		Constant name = varName;
		chunk.constants.push_back(name);
		chunk.writeOpByte(token.line, OpCode::DECLARE_VAR, chunk.constants.size() - 1);
		break;
	}
	case TokenType::IDENTIFIER: {
		string varName = string(token.start, token.start + token.length);
		Constant name = varName;
		chunk.constants.push_back(name);
		chunk.writeOpByte(token.line, OpCode::GET_VAR, chunk.constants.size() - 1);
		break;
	}
	case TokenType::EQUAL:
		chunk.writeOp(token.line, OpCode::DEFINE_VAR);
		break;
	case TokenType::DOT:
		chunk.writeOp(token.line, OpCode::GET_MEMBER);
		break;
	// return
	case TokenType::RETURN:
		chunk.writeOp(token.line, OpCode::RETURN);
		break;
	// utilities
	case TokenType::POP_STACK:
		chunk.writeOp(token.line, OpCode::POP_STACK);
		break;
	default: {
		string message = string("Unimplemented token type found '" + Parser::getName(token) + "'.");
		throw exception(message.c_str());
		break;
	}
	}
}