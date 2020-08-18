#include <queue>
#include <stack>

#include "Parser.h"

using namespace std;

// API

Parser::Parser(Scanner* scanner) {
	this->ast = nullptr;
	this->scanner = scanner;
}

InterpreterResults Parser::parse() {
	return expression();
}

InterpreterResults Parser::expression() {
	queue<Token> RPN = queue<Token>();
	stack<Token> opp = stack<Token>();
	for (Token token = scanner->scanToken(); token.type != TokenType::FILE_END; token = scanner->scanToken()) {
		
	}
	return InterpreterResults::OK;
}

// Helpers

