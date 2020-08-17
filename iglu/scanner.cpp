#include "scanner.h"

#pragma region API

scanner::scanner(const string& source) {
	this->start = source.begin();
	this->current = source.begin();
	this->end = source.end();
	this->line = 0;
}

Token scanner::scanToken() {
	skipWhiteSpace();
	
	start = current;

	if(isAtEnd()) return makeToken(TokenType::FILE_END);

	char c = advance();

	switch (c)
	{
	case '(': return makeToken(TokenType::LEFT_PARAN);
	case ')': return makeToken(TokenType::RIGHT_PARAN);
	case '{': return makeToken(TokenType::RIGHT_BRACE);
	case '}': return makeToken(TokenType::LEFT_BRACE);
	case '+': return makeToken(TokenType::PLUS);
	case '-': return makeToken(TokenType::MINUS);
	case '*': return makeToken(TokenType::STAR);
	case '/': return makeToken(TokenType::SLASH);
	case ',': return makeToken(TokenType::COMMA);
	case '.': return makeToken(TokenType::DOT);
	case ';': return makeToken(TokenType::SEMICOLON);

	case '=':
		if(peek() == '=') return makeToken(TokenType::EQUAL_EQUAL);
		if(peek() == '>') return makeToken(TokenType::ARROW);
		return makeToken(TokenType::EQUAL);
	case '!':
		if(peek() == '=') return makeToken(TokenType::BANG_EQUAL);
		return makeToken(TokenType::BANG);
	case '>':
		if(peek() == '=') return makeToken(TokenType::GREATER_EQUAL);
		return makeToken(TokenType::GREATER);
	case '<':
		if(peek() == '=') return makeToken(TokenType::LESS_EQUAL);
		return makeToken(TokenType::LESS);
	}
	
	return errorToken("Unexpected character.");
}

#pragma endregion

#pragma region Control

bool scanner::isAtEnd() {
	return current == end;
}

char scanner::peek() {
	return *current;
}

char scanner::peekNext() {
	if(isAtEnd()) return '\0';
	return current[1];
}

char scanner::advance() {
	return *current++;
}

#pragma endregion

#pragma region Helpers

#pragma endregion

#pragma region TokenSpecifics
Token scanner::makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = start;
	token.length = current - start;
	token.line = line;

	return token;
}

Token scanner::errorToken(const string& message) {
	Token token;
	token.type = TokenType::ERROR;
	token.start = message.begin();
	token.length = message.size();
	token.line = line;
	return token;
}

void scanner::skipWhiteSpace() {
	while (true) {
		char c = peek();
		switch (c)
		{
		case ' ':
		case '\r':
		case '\t':
			advance();
			break;
		case '\n':
			line++;
			break;
		case '/':
			if (peekNext() == '//') {
				while (peekNext() != '/' && !isAtEnd()) advance();
			}
		default:
			break;
		}
	}
}
#pragma endregion

