#include "Parser.h"

#include <iostream>
#include <vector>

using namespace std;

// API

Parser::Parser(Scanner* scanner) {
	this->scanner = scanner;
	current = Token();
	rpn = queue<Token>();
	psm = PSM{};
	hadError = false;
}

void Parser::parse() {
	hadError = false;
	while (!scanner->isAtEnd()) {
		tryStatement();
	}
}

queue<Token>* Parser::getRPN() {
	return &rpn;
}

// specific parsers

void Parser::statement() {
	rpn = queue<Token>();
	psm.reset();
	current = scanner->scanToken();
	while (true) {
		//// check for error using Parser State Machine
		//if (!psm.next(current)) {
		//	if (psm.is_panic()) panicError("!TODO replace with new panic Error function");
		//	else continue;
		//}
		switch (current.type)
		{
			case TokenType::LET:
				// insert let token into rpn
				rpn.push(current);

				current = scanner->scanToken();

				// get assignment expression
				tryExpression(TokenType::SEMICOLON);
				break;
			default:
				tryExpression(TokenType::SEMICOLON);
				break;
		}
		
		rpn.push(utility_popToken);
		if (current.type == TokenType::FILE_END) break;
	}
}

//void Parser::statement() {
//	rpn = queue<Token>();
//	psm.reset();
//	current = scanner->scanToken();
//	while (true) {
//		if (!psm.next(current)) {
//			if (psm.is_panic()) panicError("");
//			else nonPanicError("");
//		}
//		switch (current.type)
//		{
//		case TokenType::LET:
//			// insert let token into rpn
//			rpn.push(current);
//
//			current = scanner->scanToken();
//
//			// get assignment expression
//			tryExpression(TokenType::SEMICOLON);
//			break;
//		//case TokenType::IF:
//		//	// insert if token into rpn
//		//	Token ifToken = current;
//
//		//	current = scanner->scanToken();
//		//	if(curr)
//
//		default:
//			// expression statement
//			if (current.presidence == Presidence::PRIMARY || current.presidence == Presidence::UNARY || current.type == TokenType::LEFT_PARAN) {
//				tryExpression(TokenType::SEMICOLON);
//			}
//			else {
//				panicError("Unexpected token '" + getName(current) + "'.");
//			}
//			break;
//		}
//		rpn.push(utility_popToken);
//		if (current.type == TokenType::FILE_END) break;
//	}
//}

void Parser::expression(TokenType delimiter) {
	vector<Token> opp = vector<Token>();
	psm.reset();
	psm.next(current.type);

	while (!psm.at_end()) {
		// check for state machine errors
		if (psm.is_error()) {continue; current = scanner->scanToken(); psm.next(current.type);};
		if (psm.is_panic()) panicError("");

		// check if primary presidence (identifier/constant)
		if (current.presidence == Presidence::PRIMARY) rpn.push(current);

		// check if presidence is for an operator
		else if(current.presidence != Presidence::NONE){
			// if operator stack is empty push operator directly
			if (opp.empty()) {
				opp.push_back(current);
			}
			else {
				// deal with left associative operators and equal presidence properly
				if (opp.back().presidence == current.presidence && current.leftAssoc) {
					rpn.push(opp.back());
					opp.pop_back();
					opp.push_back(current);
				}
				// if next operator has lower presicidence than top of opperator stack, clear opperator stack until
				// opperator stack contains lower (or equal if right associative operator) operator on top.
				else if (opp.back().presidence > current.presidence) {
					do {
						rpn.push(opp.back());
						opp.pop_back();
					}
					while (!opp.empty() && (opp.back().presidence > current.presidence || opp.back().presidence == current.presidence && current.leftAssoc));
					opp.push_back(current);
				}
				// if operator has higher presidence than top of operator stack then push to stack
				else {
					opp.push_back(current);
				}
			}
		}
		// deal with left parenthesies
		else if (current.type == TokenType::LEFT_PARAN) opp.push_back(current);

		// deal with right parenthesises
		else if (current.type == TokenType::RIGHT_PARAN) {
			// reset operator stack until left parenthesis found
			while (!opp.empty() && opp.back().type != TokenType::LEFT_PARAN) {
				rpn.push(opp.back());
				opp.pop_back();
			}
			// error management if unpaired right parenthesis is found
			if (opp.empty()) {
				panicError("Unpaired ')' found.");
			}

			// remove left parenthesis
			opp.pop_back();
		}

		// advance scanner and state machine
		current = scanner->scanToken();
		psm.next(current.type);
	}
	// clear remaining operators from stack
	while (!opp.empty()) {
		// deal with unparied left parenthesis
		if (opp.back().type == TokenType::LEFT_PARAN) {
			nonPanicError("Unpaired '(' found.");
		}
		rpn.push(opp.back());
		opp.pop_back();
	}

	// consume delimiter
	current = scanner->scanToken();
}

//void Parser::expression(TokenType delimiter, ExpressionType type) {
//	vector<Token> opp = vector<Token>();
//	
//	// values for validating expression
//	bool expectOper = false;
//	bool allowAssignment = true;
//
//	for (; current.type != delimiter; current = scanner->scanToken()) {
//		// check if primary presidence (identifier/constant)
//		if (current.presidence == Presidence::PRIMARY){
//			// error if expected an operator
//			if(expectOper) nonPanicError("Expected an operator, found '" + getName(current) + "'.");
//			
//			expectOper = true;
//			if(current.type != TokenType::IDENTIFIER) allowAssignment = false;
//
//			rpn.push(current);
//		}
//		// check if presidence is for an operator
//		else if(current.presidence != Presidence::NONE){
//			// report error if operator was not expected (unary opperators are allowed)
//			if (current.presidence != Presidence::UNARY && !expectOper) nonPanicError("Expected identifier, constant, or unary operator, found '" + getName(current) + "'.");
//			// report error if assignment not allowed
//			if (current.presidence == Presidence::ASSIGNMENT){
//				if(type != ExpressionType::VAR_DEC && !allowAssignment) nonPanicError("Cannot assign to the left hand side of '=' operator.");
//				if(type == ExpressionType::VAR_DEC && !allowAssignment) nonPanicError("Cannot declare variable on the left hand side of the '=' operator.");
//			}
//
//			// update if expectation values 
//			expectOper = false;
//			if (current.presidence < Presidence::CALL ||
//				type != ExpressionType::VAR_DEC && (current.type != TokenType::IDENTIFIER && current.type != TokenType::DOT) ||
//				type == ExpressionType::VAR_DEC && (current.type != TokenType::IDENTIFIER && current.type != TokenType::DOT))
//			{
//				allowAssignment = false;
//			}
//
//			// if operator stack is empty push operator directly
//			if (opp.empty()) {
//				opp.push_back(current);
//			}
//			else {
//				// deal with left associative operators and equal presidence properly
//				if (opp.back().presidence == current.presidence && current.leftAssoc) {
//					rpn.push(opp.back());
//					opp.pop_back();
//					opp.push_back(current);
//				}
//				// if next operator has lower presicidence than top of opperator stack, clear opperator stack until
//				// opperator stack contains lower (or equal if right associative operator) operator on top.
//				else if (opp.back().presidence > current.presidence) {
//					do {
//						rpn.push(opp.back());
//						opp.pop_back();
//					}
//					while (!opp.empty() && (opp.back().presidence > current.presidence || opp.back().presidence == current.presidence && current.leftAssoc));
//					opp.push_back(current);
//				}
//				// if operator has higher presidence than top of operator stack then push to stack
//				else {
//					opp.push_back(current);
//				}
//			}
//		}
//		// deal with left parenthesies
//		else if (current.type == TokenType::LEFT_PARAN) {
//			if (expectOper) nonPanicError("Expected an operator, found '" + getName(current) + "'.");
//			expectOper = false;
//			allowAssignment = false;
//
//			opp.push_back(current);
//		}
//		// deal with right parenthesises
//		else if (current.type == TokenType::RIGHT_PARAN) {
//			if (!expectOper) nonPanicError("Expected identifier or constant, found '" + getName(current) + "'.");
//			expectOper = true;
//			allowAssignment = false;
//
//			// reset operator stack until left parenthesis found
//			while (!opp.empty() && opp.back().type != TokenType::LEFT_PARAN) {
//				rpn.push(opp.back());
//				opp.pop_back();
//			}
//			// error management if unpaired right parenthesis is found
//			if (opp.empty()) {
//				panicError("Unpaired ')' found.");
//			}
//
//			// remove left parenthesis
//			opp.pop_back();
//		}
//		// handle missing delimiters at file end or unexpected tokens
//		else if (current.type == TokenType::FILE_END) {
//			panicError("Expected '" + getName(delimiter) + "' but reached end of file instead.");
//		}
//		else{
//			panicError("Expected '" + getName(delimiter) + "' but found '" + getName(current) + "' instead.");
//		}
//	}
//	// clear remaining operatros from stack
//	while (!opp.empty()) {
//		// deal with unparied left parenthesis
//		if (opp.back().type == TokenType::LEFT_PARAN) {
//			nonPanicError("Unpaired '(' found.");
//		}
//		rpn.push(opp.back());
//		opp.pop_back();
//	}
//
//	// consume delimiter
//	current = scanner->scanToken();
//}

// parser statemachine

PSM::PSM() {
	reset();
}

void PSM::next(TokenType type) {
	vector<pair<TokenType,State>> validTypes;
	string errorMessage;
	switch (state)
	{
	case State::START:
		validTypes = {
			{delimiter, State::END},

			{TokenType::LEFT_PARAN, State::LEFT_PARAN},

			{TokenType::NEGATE, State::UNARY},
			{TokenType::BANG, State::UNARY},

			{TokenType::STRING, State::CONST},
			{TokenType::TRUE, State::CONST},
			{TokenType::FALSE, State::CONST},
			{TokenType::NUMBER, State::CONST},
			{TokenType::NILL, State::CONST},

			{TokenType::IDENTIFIER, State::IDENT}
		};
		break;
	case State::END:
		break;
	case State::LEFT_PARAN:
		validTypes = {
			{TokenType::LEFT_PARAN, State::LEFT_PARAN},

			{TokenType::NEGATE, State::UNARY},
			{TokenType::BANG, State::UNARY},

			{TokenType::STRING, State::CONST},
			{TokenType::TRUE, State::CONST},
			{TokenType::FALSE, State::CONST},
			{TokenType::NUMBER, State::CONST},
			{TokenType::NILL, State::CONST},

			{TokenType::IDENTIFIER, State::IDENT}
		};
		break;
	case State::RIGHT_PARAN:
		validTypes = {
			{delimiter, State::END},

			{TokenType::RIGHT_PARAN, State::RIGHT_PARAN},

			{TokenType::PLUS, State::OPERATOR},
			{TokenType::MINUS, State::OPERATOR},
			{TokenType::STAR, State::OPERATOR},
			{TokenType::SLASH, State::OPERATOR},
			{TokenType::EQUAL_EQUAL, State::OPERATOR},
			{TokenType::BANG_EQUAL, State::OPERATOR},
			{TokenType::LESS, State::OPERATOR},
			{TokenType::LESS_EQUAL, State::OPERATOR},
			{TokenType::GREATER, State::OPERATOR},
			{TokenType::GREATER_EQUAL, State::OPERATOR},

			{TokenType::LEFT_PARAN, State::FN_CALL},
		};
		break;
	case State::CONST:
		validTypes = {
			{delimiter, State::END},
			
			{TokenType::RIGHT_PARAN, State::RIGHT_PARAN},

			{TokenType::PLUS, State::OPERATOR},
			{TokenType::MINUS, State::OPERATOR},
			{TokenType::STAR, State::OPERATOR},
			{TokenType::SLASH, State::OPERATOR},
			{TokenType::EQUAL_EQUAL, State::OPERATOR},
			{TokenType::BANG_EQUAL, State::OPERATOR},
			{TokenType::LESS, State::OPERATOR},
			{TokenType::LESS_EQUAL, State::OPERATOR},
			{TokenType::GREATER, State::OPERATOR},
			{TokenType::GREATER_EQUAL, State::OPERATOR},
		};
		break;
	case State::UNARY:
		validTypes = {
			{TokenType::RIGHT_PARAN, State::RIGHT_PARAN},

			{TokenType::PLUS, State::OPERATOR},
			{TokenType::MINUS, State::OPERATOR},
			{TokenType::STAR, State::OPERATOR},
			{TokenType::SLASH, State::OPERATOR},
			{TokenType::EQUAL_EQUAL, State::OPERATOR},
			{TokenType::BANG_EQUAL, State::OPERATOR},
			{TokenType::LESS, State::OPERATOR},
			{TokenType::LESS_EQUAL, State::OPERATOR},
			{TokenType::GREATER, State::OPERATOR},
			{TokenType::GREATER_EQUAL, State::OPERATOR},

			{TokenType::NEGATE, State::UNARY},
			{TokenType::BANG, State::UNARY},
		};
		break;
	case State::IDENT:
		validTypes = {
			{delimiter, State::END},
			
			{TokenType::RIGHT_PARAN, State::RIGHT_PARAN},

			{TokenType::PLUS, State::OPERATOR},
			{TokenType::MINUS, State::OPERATOR},
			{TokenType::STAR, State::OPERATOR},
			{TokenType::SLASH, State::OPERATOR},
			{TokenType::EQUAL_EQUAL, State::OPERATOR},
			{TokenType::BANG_EQUAL, State::OPERATOR},
			{TokenType::LESS, State::OPERATOR},
			{TokenType::LESS_EQUAL, State::OPERATOR},
			{TokenType::GREATER, State::OPERATOR},
			{TokenType::GREATER_EQUAL, State::OPERATOR},
			{TokenType::DOT, State::OPERATOR}, //!TODO create specific state for the DOT

			{TokenType::LEFT_PARAN, State::FN_CALL},
		};
		break;
	case State::OPERATOR:
		validTypes = {
			{TokenType::STRING, State::CONST},
			{TokenType::TRUE, State::CONST},
			{TokenType::FALSE, State::CONST},
			{TokenType::NUMBER, State::CONST},
			{TokenType::NILL, State::CONST},

			{TokenType::IDENTIFIER, State::IDENT}
		};
		break;
	case State::FN_CALL:
		validTypes = {
			{TokenType::STRING, State::PARAM},
			{TokenType::TRUE, State::PARAM},
			{TokenType::FALSE, State::PARAM},
			{TokenType::NUMBER, State::PARAM},
			{TokenType::NILL, State::PARAM},
			{TokenType::IDENTIFIER, State::PARAM},
		};
		break;
	case State::END_FN_CALL:
		validTypes = {
			{delimiter, State::END},

			{TokenType::RIGHT_PARAN, State::RIGHT_PARAN},

			{TokenType::PLUS, State::OPERATOR},
			{TokenType::MINUS, State::OPERATOR},
			{TokenType::STAR, State::OPERATOR},
			{TokenType::SLASH, State::OPERATOR},
			{TokenType::EQUAL_EQUAL, State::OPERATOR},
			{TokenType::BANG_EQUAL, State::OPERATOR},
			{TokenType::LESS, State::OPERATOR},
			{TokenType::LESS_EQUAL, State::OPERATOR},
			{TokenType::GREATER, State::OPERATOR},
			{TokenType::GREATER_EQUAL, State::OPERATOR},
			{TokenType::DOT, State::OPERATOR}, //!TODO create specific state for the DOT
		};
		break;
	case State::PARAM:
		validTypes = {
			{TokenType::COMMA, State::PARAM_COMMA},
			
			{TokenType::RIGHT_PARAN, State::END_FN_CALL},
		};
		break;
	case State::PARAM_COMMA: // !TODO think about merging this with the FN_CALL state "could be more efficient"
		validTypes = {
			{TokenType::STRING, State::PARAM},
			{TokenType::TRUE, State::PARAM},
			{TokenType::FALSE, State::PARAM},
			{TokenType::NUMBER, State::PARAM},
			{TokenType::NILL, State::PARAM},
			{TokenType::IDENTIFIER, State::PARAM},
		};
		break;
	case State::ERROR:
		// !TODO remove
		cout << "Entered Error State Parser";
		break;
	case State::PANIC_ERROR:
		break;
	default:
		break;
	}

	for (auto it = validTypes.begin(); it < validTypes.end();){
		if(it->first == type){
			state = it->second;
			break;
		}
		it++;
		if (it == validTypes.end()) {
			state = State::ERROR;
		}
	}
}

bool PSM::is_error() {
	return state == State::ERROR;
}

bool PSM::is_panic() {
	return state == State::PANIC_ERROR;
}

bool PSM::at_end() {
	return state == State::END;
}

void PSM::setAsAssignment() {
	exprType = ExprType::ASSIGN;
}

void PSM::change_delimiter(TokenType type) {
	delimiter = type;
}

void PSM::reset() {
	delimiter = TokenType::SEMICOLON;
	exprType = ExprType::DEF;

	state = State::START;

	errorMessage = "";
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

void Parser::tryStatement() {
	try {
		statement();
	}
	catch (Parser::PanicException err) {
		reachDelimiter(TokenType::SEMICOLON);
	}
}

void Parser::tryExpression(TokenType delimiter) {
	try {
		expression(delimiter);
	}
	catch (Parser::PanicException err) {
		reachDelimiter(delimiter);
	}
}