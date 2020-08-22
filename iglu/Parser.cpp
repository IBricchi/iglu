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
			current = scanner->scanToken();
			// get expression
			tryExpression(TokenType::SEMICOLON, true);

			break;
		default:
			// expression statement
			if (current.presidence == Presidence::PRIMARY || current.presidence == Presidence::UNARY || current.type == TokenType::LEFT_PARAN) {
				tryExpression(TokenType::SEMICOLON);
			}
			break;
		}
		if (current.type == TokenType::FILE_END) break;
	}
}

inline void Parser::expression(TokenType delimiter) {
	expression(delimiter, false);
}

void Parser::expression(TokenType delimiter, bool assignmentExpression) {
	stack<Token> opp = stack<Token>();
	bool expectOper = false;
	bool allowAssignment = true;
	bool isAssignment = false;
	for (; current.type != delimiter; current = scanner->scanToken()) {
		// check if primary presidence (identifier/constant)
		if (current.presidence == Presidence::PRIMARY){
			// error if expected an operator
			if(expectOper) nonPanicError("Expected an operator, found '" + getName(current) + "'.");
			
			expectOper = true;
			if(current.type != TokenType::IDENTIFIER) allowAssignment = false;

			rpn->push(current);
		}
		// check if presidence is for an operator
		else if(current.presidence != Presidence::NONE){
			// report error if operator was not expected (unary opperators are allowed)
			if (current.presidence != Presidence::UNARY && !expectOper) nonPanicError("Expected identifier, constant, or unary operator, found '" + getName(current) + "'.");
			// report error if assignment not allowed
			if (current.presidence == Presidence::ASSIGNMENT){
				if(!allowAssignment) nonPanicError("Cannot assign to the left hand side of '=' operator.");
				else isAssignment = true;
			}

			// update if expectation values 
			expectOper = false;
			if (current.presidence < Presidence::CALL || (current.type != TokenType::IDENTIFIER && current.type != TokenType::DOT)) allowAssignment = false;

			// if operator stack is empty push operator directly
			if (opp.empty()) {
				opp.push(current);
			}
			else {
				// deal with left associative operators and equal presidence properly
				if (opp.top().presidence == current.presidence && current.leftAssoc) {
					rpn->push(opp.top());
					opp.pop();
					opp.push(current);
				}
				// if next operator has lower presicidence than top of opperator stack, clear opperator stack until
				// opperator stack contains lower (or equal if right associative operator) operator on top.
				else if (opp.top().presidence > current.presidence) {
					do {
						rpn->push(opp.top());
						opp.pop();
					}
					while (!opp.empty() && (opp.top().presidence > current.presidence || opp.top().presidence == current.presidence && current.leftAssoc));
					opp.push(current);
				}
				// if operator has higher presidence than top of operator stack then push to stack
				else {
					opp.push(current);
				}
			}
		}
		// deal with left parenthesies
		else if (current.type == TokenType::LEFT_PARAN) {
			if (expectOper) nonPanicError("Expected an operator, found '" + getName(current) + "'.");
			expectOper = false;
			allowAssignment = false;

			opp.push(current);
		}
		// deal with right parenthesises
		else if (current.type == TokenType::RIGHT_PARAN) {
			if (!expectOper) nonPanicError("Expected identifier or constant, found '" + getName(current) + "'.");
			expectOper = true;
			allowAssignment = false;

			// reset operator stack until left parenthesis found
			while (!opp.empty() && opp.top().type != TokenType::LEFT_PARAN) {
				rpn->push(opp.top());
				opp.pop();
			}
			// error management if unpaired right parenthesis is found
			if (opp.empty()) {
				panicError("Unpaired ')' found.");
			}

			// remove left parenthesis
			opp.pop();
		}
		// handle missing delimiters at file end or unexpected tokens
		else if (current.type == TokenType::FILE_END) {
			panicError("Expected '" + getName(delimiter) + "' but reached end of file instead.");
		}
		else{
			panicError("Expected '" + getName(delimiter) + "' but found '" + getName(current) + "' instead.");
		}
	}
	// clear remaining operatros from stack
	while (!opp.empty()) {
		// deal with unparied left parenthesis
		if (opp.top().type == TokenType::LEFT_PARAN) {
			nonPanicError("Unpaired '(' found.");
		}
		rpn->push(opp.top());
		opp.pop();
	}

	// consume delimiter
	current = scanner->scanToken();
	
	// report error if assignment was required but not presented
	if(assignmentExpression && !isAssignment) nonPanicError("Expected valid '=' operator in expression, not found.");
}

// Errors

const char* Parser::PanicException::what() const throw() {
	return "Panic Mode Error";
}

void Parser::consume(TokenType type) {
	current = scanner->scanToken();
	if(current.type != type) panicError("Expected '" + getName(type) + "' but found '" + getName(current) + "' instead.");
}

void Parser::reachDelimiter(TokenType type) {
	while (current.type != type && current.type != TokenType::FILE_END) {
		current = scanner->scanToken();
	}
}

inline void Parser::panicError(string message) {
	nonPanicError(message);
	throw Parser::PanicException();
}

inline void Parser::nonPanicError(string message) {
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

inline void Parser::tryExpression(TokenType delimiter) {
	tryExpression(delimiter, false);
}

void Parser::tryExpression(TokenType delimiter, bool assignmentExpression) {
	try {
		expression(delimiter, assignmentExpression);
	}
	catch (Parser::PanicException err) {
		reachDelimiter(delimiter);
	}
}