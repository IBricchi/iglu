#include "AST.h"

AST::AST(AST* parent, Token token) {
	this->token = token;
	this->parent = nullptr;
	this->left = nullptr;
	this->right = nullptr;
}

AST::~AST() {
	if(this->left != nullptr) delete this->left;
	if(this->right != nullptr) delete this->right;
}

AST* AST::push_left(Token token) {
	if(this->left != nullptr) delete this->left;
	AST* left = new AST(this, token);
	this->left = left;
	return left;
}

AST* AST::push_right(Token token) {
	if(this->right != nullptr) delete this->right;
	AST* right = new AST(this, token);
	this->right = right;
	return right;
}