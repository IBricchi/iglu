#include "scanner.h"

#pragma region API

Scanner::Scanner(const string& source) {
	this->start = &source[0];
	this->current = &source[0];
	this->line = 0;
}

bool Scanner::isAtEnd() {
	return *current == '\0';
}

Token Scanner::scanToken() {
	skipWhiteSpace();
	
	start = current;

	if(isAtEnd()) return makeToken(TokenType::FILE_END);

	char c = advance();

	if(isDigit(c)) return numberToken();
	if(isAlpha(c)) return identifierToken();

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
	case '"': return stringToken();

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

char Scanner::peek() {
	return *current;
}

char Scanner::peekNext() {
	if(isAtEnd()) return '\0';
	return current[1];
}

char Scanner::advance() {
	return *current++;
}

#pragma endregion

#pragma region Helpers

bool Scanner::isDigit(char c) {
	return c <= '9' && c >= '0';
}

bool Scanner::isAlpha(char c) {
	return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >='A') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
	return isAlpha(c) || isDigit(c);
}

bool Scanner::checkKeyword(int s, int l, const char* expected) {
	return current-start==s+l &&
		memcmp(start+s, expected, l * sizeof(char)) == 0;
}

TokenType Scanner::identifierType() {
	char c = *start;
	switch (c)
	{
	case 'a':
		if(checkKeyword(1,2,"nd")) return TokenType::AND;
	}
	return TokenType::IDENTIFIER;
}

#pragma endregion

#pragma region TokenSpecifics

void Scanner::skipWhiteSpace() {
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
			return;
		}
	}
}

Token Scanner::makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = start;
	token.length = current - start;
	token.line = line;

	return token;
}

Token Scanner::errorToken(const string& message) {
	Token token;
	token.type = TokenType::ERROR;
	token.start = &message[0];
	token.length = message.size();
	token.line = line;
	return token;
}

Token Scanner::stringToken() {
	advance();
	while (!isAtEnd() && peek() != '"') {
		advance();
	}
	if (isAtEnd()) return errorToken("Unterminated string");
	advance();

	return makeToken(TokenType::STRING);
}

Token Scanner::numberToken() {
	while(isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext())) {
		advance();

		while(isDigit(peek())) advance();
	}

	return makeToken(TokenType::NUMBER);
}

Token Scanner::identifierToken() {
	while(isAlphaNumeric(peek())) advance();
	
	return makeToken(identifierType());
}

#pragma endregion

