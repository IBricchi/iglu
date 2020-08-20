#include "Parser.h"

#include <iostream>
#include <stack>

using namespace std;

// API

Parser::Parser(Scanner* scanner) {
	this->rpn = nullptr;
	this->scanner = scanner;
}

bool Parser::parse() {
	return statement();
}

queue<Token>* Parser::getRPN() {
	return rpn;
}

// specific parsers

bool Parser::statement() {
	bool out = true;
	rpn = new queue<Token>();
	for (current = scanner->scanToken(); true; current = scanner->scanToken()) {
		switch (current.type)
		{
		case TokenType::LET:
			break;
		default:
			break;
		}
		// TODO! this needs to go into a case statememnt later
		try{
			out |= expression(TokenType::SEMICOLON);
		}
		catch(Parser::PanicException err){
			out = false;
			while (current.type != TokenType::SEMICOLON && current.type != TokenType::FILE_END) {
				current = scanner->scanToken();
			}
		}
		if (current.type == TokenType::FILE_END) break;
	}
	return out;
}

bool Parser::expression(TokenType delimiter) {
	bool out = true;
	stack<Token> opp = stack<Token>();
	for (; current.type != delimiter; current = scanner->scanToken()) {
		if (current.presidence == Presidence::PRIMARY){
			rpn->push(current);
		}
		else if(current.presidence != Presidence::NONE){
			if (!opp.empty()) {
				if (opp.top().presidence == current.presidence && current.leftAssoc) {
					rpn->push(opp.top());
					opp.pop();
					opp.push(current);
				}
				else if (opp.top().presidence > current.presidence) {
					do {
						rpn->push(opp.top());
						opp.pop();
					}
					while (!opp.empty() && (opp.top().presidence > current.presidence || opp.top().presidence == current.presidence && current.leftAssoc));
					opp.push(current);
				}
				else {
					opp.push(current);
				}
			}
			else {
				opp.push(current);
			}
		}
		else if (current.type == TokenType::LEFT_PARAN) {
			opp.push(current);
		}
		else if (current.type == TokenType::RIGHT_PARAN) {
			while (opp.top().type != TokenType::LEFT_PARAN) {
				rpn->push(opp.top());
				opp.pop();
			}
			opp.pop();
		}
		else if (current.type == TokenType::FILE_END) {
			PanicError("Expected '" + Token::tokenName(delimiter) + "' but reached end of file instead.");
		}
		else{
			PanicError("Expected '" + Token::tokenName(delimiter) + "' but found '" + Token::tokenName(current.type) + "' instead.");
		}
	}
	while (!opp.empty()) {
		rpn->push(opp.top());
		opp.pop();
	}
	// consume delimiter
	current = scanner->scanToken();

	return out;
}

// Errors

const char* Parser::PanicException::what() const throw() {
	return "Panic Mode Error";
}

void Parser::PanicError(string message) {
	cerr << "[Line: " << current.line << "] " << message << endl;
	throw Parser::PanicException();
}

