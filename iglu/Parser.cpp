#include "Parser.h"

#include <iostream>
#include <stack>

using namespace std;

// API

Parser::Parser(Scanner* scanner) {
	hadError = false;
	this->rpn = nullptr;
	this->scanner = scanner;
}

void Parser::parse() {
	hadError = false;
	statement();
}

queue<Token>* Parser::getRPN() {
	return rpn;
}

// specific parsers

void Parser::statement() {
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
			expression(TokenType::SEMICOLON);
		}
		catch(Parser::PanicException err){
			while (current.type != TokenType::SEMICOLON && current.type != TokenType::FILE_END) {
				current = scanner->scanToken();
			}
		}
		if (current.type == TokenType::FILE_END) break;
	}
}

void Parser::expression(TokenType delimiter) {
	stack<Token> opp = stack<Token>();
	bool expectOper = false;
	for (; current.type != delimiter; current = scanner->scanToken()) {
		if (current.presidence == Presidence::PRIMARY){
			if(expectOper) NonPanicError("Expected an operator, found '" + getName(current.type) + "'.");
			expectOper = true;
			rpn->push(current);
		}
		else if(current.presidence != Presidence::NONE){
			if(current.presidence != Presidence::UNARY && !expectOper) NonPanicError("Expected value or unary operator, found '" + getName(current.type) + "'.");
			expectOper = false;
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
			if (expectOper) NonPanicError("Expected an operator, found '" + getName(current.type) + "'.");
			expectOper = false;
			opp.push(current);
		}
		else if (current.type == TokenType::RIGHT_PARAN) {
			if (expectOper) NonPanicError("Expected an operator, found '" + getName(current.type) + "'.");
			expectOper = true;
			while (opp.top().type != TokenType::LEFT_PARAN) {
				rpn->push(opp.top());
				opp.pop();
			}
			opp.pop();
		}
		else if (current.type == TokenType::FILE_END) {
			PanicError("Expected '" + getName(delimiter) + "' but reached end of file instead.");
		}
		else{
			PanicError("Expected '" + getName(delimiter) + "' but found '" + getName(current.type) + "' instead.");
		}
	}
	while (!opp.empty()) {
		rpn->push(opp.top());
		opp.pop();
	}
	// consume delimiter
	current = scanner->scanToken();
}

// Errors

const char* Parser::PanicException::what() const throw() {
	return "Panic Mode Error";
}

inline void Parser::PanicError(string message) {
	NonPanicError(message);
	throw Parser::PanicException();
}

inline void Parser::NonPanicError(string message) {
	hadError = true;
	cerr << "[Line: " << current.line << "] " << message << endl;
}

// Helpers

inline string Parser::getName(Token token) {
	switch (token.type)
	{
	case TokenType::LEFT_PARAN:
	case TokenType::RIGHT_PARAN:
	case TokenType::LEFT_BRACE:
	case TokenType::RIGHT_BRACE:
	case TokenType::PLUS:
	case TokenType::MINUS:
	case TokenType::STAR:
	case TokenType::SLASH:
	case TokenType::COMMA:
	case TokenType::DOT:
	case TokenType::SEMICOLON:
	case TokenType::ARROW:
	case TokenType::NEGATE:
	case TokenType::EQUAL:
	case TokenType::EQUAL_EQUAL:
	case TokenType::BANG:
	case TokenType::BANG_EQUAL:
	case TokenType::GREATER:
	case TokenType::GREATER_EQUAL:
	case TokenType::LESS:
	case TokenType::LESS_EQUAL:
	case TokenType::AND:
	case TokenType::OR:
	case TokenType::IF:
	case TokenType::ELSE:
	case TokenType::RETURN:
	case TokenType::TRUE:
	case TokenType::FALSE:
	case TokenType::FOR:
	case TokenType::WHILE:
	case TokenType::LET:
	case TokenType::FUN:
	case TokenType::NILL:
	case TokenType::THIS:
	case TokenType::PARENT:
	case TokenType::CHILD_OF:
	case TokenType::ERROR:
	case TokenType::FILE_END:
		return Token::tokenName(token.type);
	case TokenType::IDENTIFIER:
	case TokenType::STRING:
	case TokenType::NUMBER:
		return string(token.start, token.start + token.length);
	}
}

inline string Parser::getName(TokenType type) {
	return Token::tokenName(type);
}