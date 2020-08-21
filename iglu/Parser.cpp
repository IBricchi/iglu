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
			if(expectOper) NonPanicError("Expected an operator, found '" + Token::tokenName(current.type) + "'.");
			expectOper = true;
			rpn->push(current);
		}
		else if(current.presidence != Presidence::NONE){
			if(current.presidence != Presidence::UNARY && !expectOper) NonPanicError("Expected value or unary operator, found '" + Token::tokenName(current.type) + "'.");
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
			if (expectOper) NonPanicError("Expected an operator, found '" + Token::tokenName(current.type) + "'.");
			expectOper = false;
			opp.push(current);
		}
		else if (current.type == TokenType::RIGHT_PARAN) {
			if (expectOper) NonPanicError("Expected an operator, found '" + Token::tokenName(current.type) + "'.");
			expectOper = true;
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
