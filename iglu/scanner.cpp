#include "scanner.h"

#pragma region API

scanner::scanner(const string& source) {
	this->start = source.begin();
	this->current = source.begin();
	this->end = source.end();
	this->col = 0;
	this->line = 0;
}

Token scanner::scanToken() {
	skipWhiteSpace();
	
	start = current;
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
			col = 0;
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

