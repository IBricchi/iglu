#include <stack>

#include "Parser.h"

using namespace std;

// API

Parser::Parser(Scanner* scanner) {
	this->rpn = nullptr;
	this->scanner = scanner;
}

InterpreterResults Parser::parse() {
	return expression();
}

InterpreterResults Parser::expression() {
	rpn = new queue<Token>();
	stack<Token> opp = stack<Token>();
	for (Token token = scanner->scanToken(); token.type != TokenType::FILE_END; token = scanner->scanToken()) {
		if (token.presidence == Presidence::PRIMARY){
			rpn->push(token);
		}
		else if(token.presidence != Presidence::NONE){
			if (!opp.empty()) {
				if (opp.top().presidence == token.presidence && token.leftAssoc) {
					rpn->push(opp.top());
					opp.pop();
					opp.push(token);
				}
				else if (opp.top().presidence > token.presidence) {
					do {
						rpn->push(opp.top());
						opp.pop();
					}
					while (!opp.empty() && (opp.top().presidence > token.presidence || opp.top().presidence == token.presidence && token.leftAssoc));
					opp.push(token);
				}
				else {
					opp.push(token);
				}
			}
			else {
				opp.push(token);
			}
		}
		else if (token.type == TokenType::LEFT_PARAN) {
			opp.push(token);
		}
		else if (token.type == TokenType::RIGHT_PARAN) {
			while (opp.top().type != TokenType::LEFT_PARAN) {
				rpn->push(opp.top());
				opp.pop();
			}
			opp.pop();
		}
	}
	while (!opp.empty()) {
		rpn->push(opp.top());
		opp.pop();
	}
	return InterpreterResults::OK;
}

// Helpers

